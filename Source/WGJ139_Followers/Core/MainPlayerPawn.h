// © Master Kenth 2020

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "MainPlayerPawn.generated.h"

UCLASS()
class WGJ139_FOLLOWERS_API AMainPlayerPawn : public ABasePawn
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleDefaultsOnly)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly)
	class UCameraComponent* Camera;

	AMainPlayerPawn();
	void Reincarnate();

protected:
	virtual void BeginPlay() override;
	virtual void Kill(AActor* KilledBy) override;
};
