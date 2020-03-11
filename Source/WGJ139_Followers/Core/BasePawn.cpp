// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "../UI/HealthBarWidget.h"
#include "FollowersGameMode.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "MainPawnMovementComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/WidgetComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackRange = 50.0f;
	AttackCooldown = 1.0f;
	LookDir = EPawnLookDir::Right;

	Health = MaxHealth = 100;
	bDead = false;
	TakeDamageInvulnerabilityTime = 0.3f;
	LastDamageTime = 0;

	RootComponent = Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	Root->SetCollisionProfileName(TEXT("Pawn"));
	Root->InitBoxExtent(FVector(25, 25, 20));

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(Root);
	Sprite->SetRelativeRotation(FRotator::MakeFromEuler(FVector(-90, 0, 0)));
	Sprite->SetCollisionProfileName(TEXT("NoCollision"));

	AttackAnim = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AttackAnim"));
	AttackAnim->SetupAttachment(Sprite);
	AttackAnim->SetCollisionProfileName(TEXT("NoCollision"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(Root);
	HealthBar->SetCollisionProfileName(TEXT("NoCollision"));
	HealthBar->SetRelativeLocation(FVector(0, -40, 0));
	HealthBar->SetRelativeRotation(FRotator::MakeFromEuler(FVector(-90, 90, 0)));
	HealthBar->SetDrawAtDesiredSize(true);

	MainPawnMovementComponent = CreateDefaultSubobject<UMainPawnMovementComponent>(TEXT("MainPawnMovementComponent"));
}

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(AttackAnim->IsActive())
	{
		int32 frame = AttackAnim->GetPlaybackPositionInFrames();
		if(frame == AttackAnim->GetFlipbookLengthInFrames() - 1)
		{
			AttackAnim->Stop();
			AttackAnim->SetVisibility(false);
		}
	}
}

UPawnMovementComponent* ABasePawn::GetMovementComponent() const
{
	return MainPawnMovementComponent;
}

void ABasePawn::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	if(!bDead)
	{
		Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
	}
}

void ABasePawn::UpdateLookDir(float HorizontalInput)
{
	if(!FMath::IsNearlyZero(HorizontalInput))
	{
		EPawnLookDir lastLookDir = LookDir;
		LookDir = HorizontalInput > 0 ? EPawnLookDir::Right : EPawnLookDir::Left;

		if(lastLookDir != LookDir)
		{
			Sprite->AddWorldRotation(FRotator::MakeFromEuler(FVector(0, 180, 0)));
		}
	}
}

void ABasePawn::TryAttack()
{
	if(CanAttack())
	{
		TArray<FOverlapResult> overlaps;
		FVector start = GetActorLocation();
		FVector lookVec = (LookDir == EPawnLookDir::Right ? FVector::ForwardVector : FVector::BackwardVector);
		float halfRange = (AttackRange + 25) * 0.5f;
		FVector center = start + lookVec * halfRange;
		FVector halfExtent = FVector(halfRange, 50, 10);
		FCollisionShape shape = FCollisionShape::MakeBox(halfExtent);
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this);
		if(GetWorld()->OverlapMultiByChannel(overlaps, center, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, shape, queryParams))
		{
			TArray<IDamageableInterface*> uniqueDamageables;
			for(FOverlapResult& overlap : overlaps)
			{
				if(overlap.Actor.IsValid())
				{
					IDamageableInterface* damageable = Cast<IDamageableInterface>(overlap.GetActor());
					if(damageable != nullptr)
					{
						ABasePawn* pawn = Cast<ABasePawn>(overlap.GetActor());
						if(!pawn || pawn->CultID != this->CultID)
						{
							uniqueDamageables.AddUnique(damageable);
						}
					}
				}
			}

			for(IDamageableInterface* damageable : uniqueDamageables)
			{
					damageable->TakeDamage(15, this);
			}
		}

		AttackAnim->SetVisibility(true);
		AttackAnim->PlayFromStart();

		LastAttackTime = GetWorld()->GetTimeSeconds();
	}
}

bool ABasePawn::CanAttack() const
{
	return GetWorld()->GetTimeSeconds() - LastAttackTime >= AttackCooldown;
}

void ABasePawn::TakeDamage(int32 Damage, AActor* DamageInstigator)
{
	float currentWorldTime = GetWorld()->GetTimeSeconds();
	if(currentWorldTime - LastDamageTime >= TakeDamageInvulnerabilityTime)
	{
		int32 newHealth = Health - Damage;

		Health = FMath::Clamp(newHealth, 0, MaxHealth);
		LastDamageTime = currentWorldTime;

		UpdateHealthBarDisplay();

		if(newHealth <= 0)
		{
			DeathEvent.Broadcast();
			Kill(DamageInstigator);
		}
		else if(DynamicMaterialInstance)
		{
			DynamicMaterialInstance->SetScalarParameterValue(TEXT("BlinkAmount"), 1.0f);

			auto stopBlinkDelegate = FTimerDelegate::CreateLambda([this]()
			{
				if(DynamicMaterialInstance)
				{
					DynamicMaterialInstance->SetScalarParameterValue(TEXT("BlinkAmount"), 0.0f);
				}
			});

			FTimerHandle handle;
			GetWorldTimerManager().SetTimer(handle, stopBlinkDelegate, TakeDamageInvulnerabilityTime, false);
		}
	}
}

void ABasePawn::SetHealth(int32 NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0, MaxHealth);
	UpdateHealthBarDisplay();
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	AttackAnim->Stop();
	AttackAnim->SetVisibility(false);
	AttackAnim->SetRelativeLocation(FVector(AttackRange, 0, 0));
	UpdateHealthBarDisplay();
}

void ABasePawn::Kill(AActor* KilledBy)
{
	Health = 0;
	bDead = true;
	Destroy();

	AFollowersGameMode* gameMode = GetWorld()->GetAuthGameMode<AFollowersGameMode>();
	if(gameMode)
	{
		gameMode->AddFollowerForNextRound(CultID, -1);

		ABasePawn* killer = Cast<ABasePawn>(KilledBy);
		if(killer)
		{
			gameMode->AddFollowerForNextRound(killer->CultID, 1);
		}
	}
}

void ABasePawn::UpdateHealthBarDisplay()
{
	HealthBar->SetVisibility(Health > 0 && Health < MaxHealth);
	
	UHealthBarWidget* widget = Cast<UHealthBarWidget>(HealthBar->GetUserWidgetObject());
	if(widget != nullptr)
	{
		widget->SetHealthPercent((float)Health / MaxHealth);
	}
}

void ABasePawn::SetMaterial(UMaterialInstanceDynamic* MID)
{
	DynamicMaterialInstance = MID;
	Sprite->SetMaterial(0, DynamicMaterialInstance);
}

void ABasePawn::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	//UE_LOG(LogTemp, Log, TEXT("This (%s) got overlap by %s"), *this->GetName(), *OtherActor->GetName());
}

void ABasePawn::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	//UE_LOG(LogTemp, Log, TEXT("This (%s) got hit by %s"), *this->GetName(), *Other->GetName());
}
