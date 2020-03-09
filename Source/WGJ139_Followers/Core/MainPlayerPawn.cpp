// Fill out your copyright notice in the Description page of Project Settings.


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