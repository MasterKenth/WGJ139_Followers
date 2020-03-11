// © Master Kenth 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MainPawnMovementComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMainPawnMovement, Log, All);
/**
 * 
 */
UCLASS()
class WGJ139_FOLLOWERS_API UMainPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float BaseMoveSpeed;

	UMainPawnMovementComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void AddInputVector(FVector WorldVector, bool bForce = false) override;
};
