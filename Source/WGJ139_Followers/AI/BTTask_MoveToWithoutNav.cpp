// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToWithoutNav.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Core/BasePawn.h"
#include "../Gameplay/NPCPawn.h"
#include "../Gameplay/NPCController.h"
#include "../Core/MainPawnMovementComponent.h"


UBTTask_MoveToWithoutNav::UBTTask_MoveToWithoutNav()
{
  BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToWithoutNav, BlackboardKey), ABasePawn::StaticClass());
  bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToWithoutNav::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  ANPCPawn* pawn = GetNPCPawn(OwnerComp);
  ABasePawn* targetPawn = GetTargetPawn(OwnerComp);
  if(pawn && targetPawn)
  {
    UMainPawnMovementComponent* pawnMovement = Cast<UMainPawnMovementComponent>(pawn->GetMovementComponent());
    if(pawnMovement)
    {
      if(FVector::DistSquared2D(pawn->GetActorLocation(), targetPawn->GetActorLocation()) < pawnMovement->BaseMoveSpeed * pawnMovement->BaseMoveSpeed)
      {
        return EBTNodeResult::Succeeded;
      }
      return EBTNodeResult::InProgress;
    }
  }
  return EBTNodeResult::Failed;
}

void UBTTask_MoveToWithoutNav::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  ANPCPawn* pawn = GetNPCPawn(OwnerComp);
  ABasePawn* targetPawn = GetTargetPawn(OwnerComp);
  if(pawn && targetPawn)
  {
    UMainPawnMovementComponent* pawnMovement = Cast<UMainPawnMovementComponent>(pawn->GetMovementComponent());
    if(pawnMovement)
    {
      FVector currentLocation = pawn->GetActorLocation();
      FVector targetLocation = targetPawn->GetActorLocation();
      if(FVector::DistSquared2D(currentLocation, targetLocation) < pawnMovement->BaseMoveSpeed * pawnMovement->BaseMoveSpeed)
      {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
      }
      else
      {
        FVector dir = (targetLocation - currentLocation).GetSafeNormal2D();
        pawn->AddMovementInput(dir);
        pawn->UpdateLookDir(dir.X);
      }
    }
  }
}

ANPCPawn* UBTTask_MoveToWithoutNav::GetNPCPawn(UBehaviorTreeComponent& OwnerComp) const
{
  ANPCController* npc = Cast<ANPCController>(OwnerComp.GetOwner());
  if(npc)
  {
    ANPCPawn* pawn = Cast<ANPCPawn>(npc->GetPawn());
    return pawn;
  }

  return nullptr;
}

ABasePawn* UBTTask_MoveToWithoutNav::GetTargetPawn(UBehaviorTreeComponent& OwnerComp) const
{
  UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent();
  if(blackboardComp)
  {
    return Cast<ABasePawn>(blackboardComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID()));
  }

  return nullptr;
}