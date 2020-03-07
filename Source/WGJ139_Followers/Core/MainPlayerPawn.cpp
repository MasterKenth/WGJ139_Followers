// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerPawn.h"
#include "MainPawnMovementComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AMainPlayerPawn::AMainPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackCooldown = 1.0f;
	LookDir = EPawnLookDir::Right;

	RootComponent = Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(Root);
	Sprite->SetRelativeRotation(FRotator::MakeFromEuler(FVector(-90, 0, 0)));

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
		UE_LOG(LogTemp, Log, TEXT("Attack!"));

		TArray<FHitResult> hits;
		FVector start = GetActorLocation();
		FVector end = start + (LookDir == EPawnLookDir::Right ? FVector::ForwardVector : FVector::BackwardVector) * 100;
		FCollisionShape shape = FCollisionShape::MakeBox(FVector(10, 25, 10));
		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(this);
		if(GetWorld()->SweepMultiByChannel(hits, start, end, FQuat::Identity, ECollisionChannel::ECC_Visibility, shape, queryParams))
		{
			UE_LOG(LogTemp, Log, TEXT("Got %d hits!"), hits.Num());
		}

		DrawDebugBox(GetWorld(), start, shape.GetExtent(), FColor::Blue, false, 1.0f);
		DrawDebugBox(GetWorld(), end, shape.GetExtent(), FColor::Red, false, 1.0f);

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
}