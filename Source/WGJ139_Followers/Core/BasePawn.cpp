// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Engine/World.h"
#include "MainPawnMovementComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "DrawDebugHelpers.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackRange = 50.0f;
	AttackCooldown = 1.0f;
	LookDir = EPawnLookDir::Right;

	Health = MaxHealth = 100;

	RootComponent = Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(Root);
	Sprite->SetRelativeRotation(FRotator::MakeFromEuler(FVector(-90, 0, 0)));
	Sprite->SetCollisionProfileName(TEXT("Pawn"));

	AttackAnim = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AttackAnim"));
	AttackAnim->SetupAttachment(Sprite);

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
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
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
			for(FOverlapResult& overlap : overlaps)
			{
				if(overlap.Actor.IsValid())
				{
					IDamageableInterface* damageable = Cast<IDamageableInterface>(overlap.GetActor());
					if(damageable != nullptr)
					{
						UE_LOG(LogTemp, Log, TEXT("Overlap %s"), *overlap.GetActor()->GetName());
						damageable->TakeDamage(15);
					}
				}
			}
		}

		DrawDebugSphere(GetWorld(), center, 10, 8, FColor::Red, false, 1.0f);
		DrawDebugBox(GetWorld(), center, shape.GetExtent(), FColor::Blue, false, 1.0f);

		AttackAnim->SetVisibility(true);
		AttackAnim->PlayFromStart();

		LastAttackTime = GetWorld()->GetTimeSeconds();
	}
}

bool ABasePawn::CanAttack() const
{
	return GetWorld()->GetTimeSeconds() - LastAttackTime >= AttackCooldown;
}

void ABasePawn::TakeDamage(int32 Damage)
{
	int32 newHealth = Health - Damage;
	UE_LOG(LogTemp, Log, TEXT("TakeDamage %d -> %d"), Health, newHealth);

	Health = FMath::Clamp(newHealth, 0, MaxHealth);
	if(newHealth <= 0)
	{
		Kill();
		DeathEvent.Broadcast();
	}
}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	AttackAnim->Stop();
	AttackAnim->SetVisibility(false);
	AttackAnim->SetRelativeLocation(FVector(AttackRange, 0, 0));
}

void ABasePawn::Kill()
{
	Health = 0;
	Destroy();
	UE_LOG(LogTemp, Log, TEXT("Dead"));
}



void ABasePawn::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	UE_LOG(LogTemp, Log, TEXT("This (%s) got overlap by %s"), *this->GetName(), *OtherActor->GetName());
}

void ABasePawn::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	UE_LOG(LogTemp, Log, TEXT("This (%s) got hit by %s"), *this->GetName(), *Other->GetName());
}