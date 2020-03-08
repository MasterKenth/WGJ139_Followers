// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerPawn.h"
#include "MainPawnMovementComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AMainPlayerPawn::AMainPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackRange = 50.0f;
	AttackCooldown = 1.0f;
	LookDir = EPawnLookDir::Right;

	RootComponent = Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(Root);
	Sprite->SetRelativeRotation(FRotator::MakeFromEuler(FVector(-90, 0, 0)));

	AttackAnim = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AttackAnim"));
	AttackAnim->SetupAttachment(Sprite);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(Root);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator::MakeFromEuler(FVector(-90, 0, -90)));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	Camera->SetOrthoWidth(1000.0f);

	MainPawnMovementComponent = CreateDefaultSubobject<UMainPawnMovementComponent>(TEXT("MainPawnMovementComponent"));
}

void AMainPlayerPawn::Tick(float DeltaTime)
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

UPawnMovementComponent* AMainPlayerPawn::GetMovementComponent() const
{
	return MainPawnMovementComponent;
}

void AMainPlayerPawn::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}

void AMainPlayerPawn::UpdateLookDir(float HorizontalInput)
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

void AMainPlayerPawn::TryAttack()
{
	if(CanAttack())
	{
		TArray<FHitResult> hits;
		FVector start = GetActorLocation();
		FVector end = start + (LookDir == EPawnLookDir::Right ? FVector::ForwardVector : FVector::BackwardVector) * AttackRange;
		FCollisionShape shape = FCollisionShape::MakeBox(FVector(20, 50, 10));
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this);
		if(GetWorld()->SweepMultiByChannel(hits, start, end, FQuat::Identity, ECollisionChannel::ECC_Visibility, shape, queryParams))
		{
			UE_LOG(LogTemp, Log, TEXT("Got %d hits!"), hits.Num());
		}

		AttackAnim->SetVisibility(true);
		AttackAnim->PlayFromStart();

		LastAttackTime = GetWorld()->GetTimeSeconds();
	}
}

bool AMainPlayerPawn::CanAttack() const
{
	return GetWorld()->GetTimeSeconds() - LastAttackTime >= AttackCooldown;
}


void AMainPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	AttackAnim->Stop();
	AttackAnim->SetVisibility(false);
	AttackAnim->SetRelativeLocation(FVector(AttackRange, 0, 0));
}