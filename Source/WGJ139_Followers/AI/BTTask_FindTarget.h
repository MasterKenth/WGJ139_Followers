// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindTarget.generated.h"

/**
 * 
 */
UCLASS()
class WGJ139_FOLLOWERS_API UBTTask_FindTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_FindTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}
};
