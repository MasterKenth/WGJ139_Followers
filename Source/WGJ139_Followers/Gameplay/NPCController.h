// © Master Kenth 2020

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
