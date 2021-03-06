// © Master Kenth 2020


#include "BTTask_MoveToWithoutNav.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Core/BasePawn.h"
#include "../Gameplay/NPCPawn.h"
#include "../Gameplay/NPCController.h"
#include "../Core/MainPawnMovementComponent.h"
#include "DrawDebugHelpers.h"


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
      FVector currentLocation = pawn->GetActorLocation();
      FVector targetLocation = targetPawn->GetActorLocation();
      if(FVector::DistSquared2D(currentLocation, targetLocation) < pawnMovement->BaseMoveSpeed * pawnMovement->BaseMoveSpeed)
      {
        FVector dir = (targetLocation - currentLocation).GetSafeNormal2D();
        pawn->UpdateLookDir(dir.X);
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
  if(pawn && targetPawn && !pawn->IsDead() && !targetPawn->IsDead())
  {
    UMainPawnMovementComponent* pawnMovement = Cast<UMainPawnMovementComponent>(pawn->GetMovementComponent());
    if(pawnMovement)
    {
      FVector currentLocation = pawn->GetActorLocation();
      FVector targetLocation = targetPawn->GetActorLocation();
      if(FVector::DistSquared2D(currentLocation, targetLocation) < 75 * 75)
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
  else
  {
    FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
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