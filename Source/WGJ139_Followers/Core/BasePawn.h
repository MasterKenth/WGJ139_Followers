// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Gameplay/DamageableInterface.h"
#include "BasePawn.generated.h"

UENUM()
enum class EPawnLookDir : uint8
{
	Right,
	Left
};

DECLARE_EVENT(ABasePawn, FDeathEvent);

UCLASS()
class WGJ139_FOLLOWERS_API ABasePawn : public APawn, public IDamageableInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float AttackRange;

	UPROPERTY(EditAnywhere)
	float AttackCooldown;

	UPROPERTY(VisibleDefaultsOnly)
	class UBoxComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
	class UPaperSpriteComponent* Sprite;
	
	UPROPERTY(VisibleDefaultsOnly)
	class UPaperFlipbookComponent* AttackAnim;
	
	UPROPERTY(VisibleDefaultsOnly)
	class UWidgetComponent* HealthBar;
	
	UPROPERTY(VisibleDefaultsOnly)
	class UMainPawnMovementComponent* MainPawnMovementComponent;

	UPROPERTY(EditAnywhere)
	int32 CultID;
	
	ABasePawn();
	virtual void Tick(float DeltaTime) override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	void UpdateLookDir(float HorizontalInput);
	void TryAttack();
	bool CanAttack() const;
	
	virtual void TakeDamage(int32 Damage) override;
	FDeathEvent& OnDeath() { return DeathEvent; }
	bool IsDead() const { return bDead; }

	void SetMaterial(class UMaterialInstanceDynamic* MID);

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	UPROPERTY()
	class UMaterialInstanceDynamic* DynamicMaterialInstance;

	float LastAttackTime;
	EPawnLookDir LookDir;

	bool bDead;
	int32 Health;
	int32 MaxHealth;
	FDeathEvent DeathEvent;

	virtual void BeginPlay() override;
	virtual void Kill();
	
	void UpdateHealthBarDisplay();
};
