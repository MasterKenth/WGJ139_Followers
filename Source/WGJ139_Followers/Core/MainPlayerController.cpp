// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "MainPlayerPawn.h"

AMainPlayerController::AMainPlayerController()
{
  MainPlayerPawn = nullptr;
}

void AMainPlayerController::BeginPlay()
{
  Super::BeginPlay();
}

void AMainPlayerController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAxis(TEXT("MoveRight"), this, &AMainPlayerController::OnInput_MoveRight);
  InputComponent->BindAxis(TEXT("MoveUp"), this, &AMainPlayerController::OnInput_MoveUp);
}

void AMainPlayerController::OnPossess(APawn* aPawn)
{
  Super::OnPossess(aPawn);
  MainPlayerPawn = Cast<AMainPlayerPawn>(aPawn);
  UE_LOG(LogTemp, Log, TEXT("Possessed pawn? %d"), MainPlayerPawn != nullptr ? 1 : 0);
}


void AMainPlayerController::OnInput_MoveRight(float Value)
{
  if(MainPlayerPawn != nullptr)
  {
    MainPlayerPawn->AddMovementInput(FVector::ForwardVector, Value);
  }
}

void AMainPlayerController::OnInput_MoveUp(float Value)
{
  if(MainPlayerPawn != nullptr)
  {
    MainPlayerPawn->AddMovementInput(-FVector::RightVector, Value);
  }
}
