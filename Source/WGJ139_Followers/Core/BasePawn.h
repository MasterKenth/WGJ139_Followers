// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UENUM()
enum class EPawnLookDir : uint8
{
	Right,
	Left
};

UCLASS()
class WGJ139_FOLLOWERS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float AttackRange;

	UPROPERTY(EditAnywhere)
	float AttackCooldown;

	UPROPERTY(VisibleDefaultsOnly)
	class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
	class UPaperSpriteComponent* Sprite;
	
	UPROPERTY(VisibleDefaultsOnly)
	class UPaperFlipbookComponent* AttackAnim;
	
	UPROPERTY(VisibleDefaultsOnly)
	class UMainPawnMovementComponent* MainPawnMovementComponent;
	
	ABasePawn();
	virtual void Tick(float DeltaTime) override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	void UpdateLookDir(float HorizontalInput);
	void TryAttack();
	bool CanAttack() const;

protected:
	float LastAttackTime;
	EPawnLookDir LookDir;
	virtual void BeginPlay() override;
};
