// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIModule/Classes/BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToWithoutNav.generated.h"

/**
 * 
 */
UCLASS()
class WGJ139_FOLLOWERS_API UBTTask_MoveToWithoutNav : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToWithoutNav();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	class ANPCPawn* GetNPCPawn(UBehaviorTreeComponent& OwnerComp) const;
	class ABasePawn* GetTargetPawn(UBehaviorTreeComponent& OwnerComp) const;
};
