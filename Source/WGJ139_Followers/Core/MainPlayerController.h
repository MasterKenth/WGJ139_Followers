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
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameOverWidget> GameOverWidgetClass;

	AMainPlayerController();

protected:
	UPROPERTY()
	class AMainPlayerPawn* MainPlayerPawn;

	UPROPERTY()
	class UGameOverWidget* GameOverWidget;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;

	void OnInput_MoveRight(float Value);
	void OnInput_MoveUp(float Value);
	void OnInput_Attack();

	void OnPlayerKilled();
};
