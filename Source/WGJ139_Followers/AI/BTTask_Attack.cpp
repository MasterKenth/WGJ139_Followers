// © Master Kenth 2020


#include "BTTask_Attack.h"
#include "../Core/BasePawn.h"
#include "../Gameplay/NPCPawn.h"
#include "../Gameplay/NPCController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_Attack::UBTTask_Attack()
{
  BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Attack, BlackboardKey), ABasePawn::StaticClass());
  bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  ANPCPawn* pawn = GetNPCPawn(OwnerComp);
  if(pawn)
  {
    if(pawn->CanAttack())
    {
      pawn->TryAttack();
      return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::InProgress;
  }

  return EBTNodeResult::Failed;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  ANPCPawn* pawn = GetNPCPawn(OwnerComp);
  if(pawn)
  {
    if(pawn->CanAttack())
    {
      pawn->TryAttack();
      FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
  }
}

ANPCPawn* UBTTask_Attack::GetNPCPawn(UBehaviorTreeComponent& OwnerComp) const
{
  ANPCController* npc = Cast<ANPCController>(OwnerComp.GetOwner());
  if(npc)
  {
    ANPCPawn* pawn = Cast<ANPCPawn>(npc->GetPawn());
    return pawn;
  }

  return nullptr;
}