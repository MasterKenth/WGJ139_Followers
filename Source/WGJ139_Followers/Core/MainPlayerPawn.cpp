// © Master Kenth 2020


#include "MainPlayerPawn.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AMainPlayerPawn::AMainPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(Root);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator::MakeFromEuler(FVector(-90, 0, -90)));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	Camera->SetOrthoWidth(1000.0f);
}

void AMainPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	SetMaterial(UMaterialInstanceDynamic::Create(Sprite->GetMaterial(0), this));
}

void AMainPlayerPawn::Reincarnate()
{
	Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Sprite->SetVisibility(true);
	Health = MaxHealth;
	bDead = false;
}

void AMainPlayerPawn::Kill(AActor* KilledBy)
{
	// Just hide the player instead to preserve camera
	Root->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sprite->SetVisibility(false);
	Health = 0;
	bDead = true;
}