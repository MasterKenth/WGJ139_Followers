// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCController.generated.h"

/**
 * 
 */
UCLASS()
class WGJ139_FOLLOWERS_API ANPCController : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BehaviorTree;
	
protected:
	virtual void BeginPlay() override;
};
