// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "MainPlayerPawn.h"
#include "../UI/GameOverWidget.h"
#include "../UI/RoundResultWidget.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "FollowersGameMode.h"

AMainPlayerController::AMainPlayerController()
{
  MainPlayerPawn = nullptr;
}

void AMainPlayerController::BeginPlay()
{
  Super::BeginPlay();

  AFollowersGameMode* gameMode = GetWorld()->GetAuthGameMode<AFollowersGameMode>();
  if(gameMode)
  {
    gameMode->OnRoundBegin().AddUObject(this, &AMainPlayerController::OnRoundBegin);
    gameMode->OnRoundEnd().AddUObject(this, &AMainPlayerController::OnRoundEnd);
  }
}

void AMainPlayerController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAxis(TEXT("MoveRight"), this, &AMainPlayerController::OnInput_MoveRight);
  InputComponent->BindAxis(TEXT("MoveUp"), this, &AMainPlayerController::OnInput_MoveUp);
  InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMainPlayerController::OnInput_Attack);
}

void AMainPlayerController::OnPossess(APawn* aPawn)
{
  Super::OnPossess(aPawn);
  MainPlayerPawn = Cast<AMainPlayerPawn>(aPawn);
  if(MainPlayerPawn)
  {
    MainPlayerPawn->OnDeath().AddUObject(this, &AMainPlayerController::OnPlayerKilled);
  }
}

void AMainPlayerController::OnInput_MoveRight(float Value)
{
  if(MainPlayerPawn != nullptr)
  {
    MainPlayerPawn->AddMovementInput(FVector::ForwardVector, Value);
    MainPlayerPawn->UpdateLookDir(Value);
  }
}

void AMainPlayerController::OnInput_MoveUp(float Value)
{
  if(MainPlayerPawn != nullptr)
  {
    MainPlayerPawn->AddMovementInput(-FVector::RightVector, Value);
  }
}

void AMainPlayerController::OnInput_Attack()
{
  if(MainPlayerPawn != nullptr)
  {
    MainPlayerPawn->TryAttack();
  }
}

void AMainPlayerController::OnPlayerKilled()
{
  UE_LOG(LogTemp, Log, TEXT("Game Over"));
  if(GameOverWidgetClass)
  {
    GameOverWidget = CreateWidget<UGameOverWidget>(this, GameOverWidgetClass);
    if(GameOverWidget)
    {
      GameOverWidget->AddToViewport();

      FInputModeUIOnly newInputMode;
      SetInputMode(newInputMode);

      bShowMouseCursor = true;
    }
  }

  AFollowersGameMode* gameMode = GetWorld()->GetAuthGameMode<AFollowersGameMode>();
  if(gameMode)
  {
    gameMode->StopGameplay();
  }
}

void AMainPlayerController::OnRoundBegin()
{
  if(RoundResultWidget)
  {
    RoundResultWidget->RemoveFromViewport();
  } 
}

void AMainPlayerController::OnRoundEnd()
{
  if(RoundResultWidgetClass)
  {
    RoundResultWidget = CreateWidget<URoundResultWidget>(this, RoundResultWidgetClass);
    if(RoundResultWidget)
    {
      RoundResultWidget->UpdateCultsDisplay();
      RoundResultWidget->AddToViewport();
    }
  }
}
