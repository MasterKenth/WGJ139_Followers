// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class WGJ139_FOLLOWERS_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainPlayerController();

protected:
	UPROPERTY()
	class AMainPlayerPawn* MainPlayerPawn;

	virtual void BeginPlay();
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;

	void OnInput_MoveRight(float Value);
	void OnInput_MoveUp(float Value);
	void OnInput_Attack();
};
