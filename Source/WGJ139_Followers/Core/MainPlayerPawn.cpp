// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerPawn.h"
#include "MainPawnMovementComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AMainPlayerPawn::AMainPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

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

void AMainPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}