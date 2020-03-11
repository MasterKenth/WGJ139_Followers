// © Master Kenth 2020


#include "NPCController.h"


void ANPCController::BeginPlay()
{
  Super::BeginPlay();
  
  if(BehaviorTree)
  {
    RunBehaviorTree(BehaviorTree);
  }
}