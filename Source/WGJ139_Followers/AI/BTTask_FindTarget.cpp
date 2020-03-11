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

          ABasePawn* playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn<ABasePawn>();
          if(playerPawn && pawn->CultID != playerPawn->CultID && FMath::RandRange(0.0f, 1.0f) <= 0.1f) // 10% chance to always target player
          {
            closestTarget = playerPawn;
          }
          else
          {
            for(TActorIterator<ABasePawn> itr(pawn->GetWorld()); itr; ++itr)
            {
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
          }

          if(closestTarget)
          {
            blackboardComp->SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), closestTarget);
            return EBTNodeResult::Succeeded;
          }
        }
      }
    }
  }
  
  return EBTNodeResult::Failed;
}