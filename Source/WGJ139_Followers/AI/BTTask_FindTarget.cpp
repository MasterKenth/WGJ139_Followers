// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindTarget.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../Gameplay/NPCController.h"
#include "../Gameplay/NPCPawn.h"
#include "../Core/BasePawn.h"
#include "../Core/FollowersGameState.h"
#include "EngineUtils.h"

UBTTask_FindTarget::UBTTask_FindTarget()
{
  BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindTarget, BlackboardKey), ABasePawn::StaticClass());
}

EBTNodeResult::Type UBTTask_FindTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  UBlackboardComponent* blackboardComp = OwnerComp.GetBlackboardComponent();
  if(blackboardComp)
  {
    ABasePawn* existingPawn = Cast<ABasePawn>(blackboardComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID()));
    if(existingPawn && !existingPawn->IsDead())
    {
      return EBTNodeResult::Succeeded;
    }
    else
    {
      ANPCController* npc = Cast<ANPCController>(OwnerComp.GetOwner());
      if(npc)
      {
        ANPCPawn* pawn = Cast<ANPCPawn>(npc->GetPawn());
        if(pawn)
        {
          float closestDistance = 0;
          ABasePawn* closestTarget = nullptr;
          const FVector selfLocation = pawn->GetActorLocation();

          TArray<ABasePawn*> bp;
          for(TActorIterator<ABasePawn> itr(pawn->GetWorld()); itr; ++itr)
          {
            bp.Add(*itr);
            continue;
            ABasePawn* possibleTarget = *itr;
            if(possibleTarget->CultID != pawn->CultID)
            {
              float distance = FVector::DistSquared2D(selfLocation, possibleTarget->GetActorLocation());
              if(distance < closestDistance || !closestTarget)
              {
                closestDistance = distance;
                closestTarget = possibleTarget;
              }
            }
          }

          if(bp.Num() > 0)//if(closestTarget)
          {
            //blackboardComp->SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), closestTarget);
            blackboardComp->SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), bp[FMath::RandRange(0, bp.Num() - 1)]);
            return EBTNodeResult::Succeeded;
          }
        }
      }
    }
  }
  
  return EBTNodeResult::Failed;
}