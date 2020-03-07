// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPlayerPawn.generated.h"

UCLASS()
class WGJ139_FOLLOWERS_API AMainPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly)
	class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
	class UPaperSpriteComponent* Sprite;

	UPROPERTY(VisibleDefaultsOnly)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly)
	class UCameraComponent* Camera;
	
	UPROPERTY(VisibleDefaultsOnly)
	class UMainPawnMovementComponent* MainPawnMovementComponent;

	AMainPlayerPawn();
	virtual void Tick(float DeltaTime) override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

protected:
	virtual void BeginPlay() override;
};
