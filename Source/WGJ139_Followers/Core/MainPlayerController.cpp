// © Master Kenth 2020


#include "MainPlayerController.h"
#include "MainPlayerPawn.h"
#include "../UI/GameOverWidget.h"
#include "../UI/RoundResultWidget.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"
#include "FollowersGameMode.h"
#include "GenericPlatform/GenericPlatformMisc.h"

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
    gameMode->OnGameStart().AddUObject(this, &AMainPlayerController::OnGameStart);
    gameMode->OnRoundBegin().AddUObject(this, &AMainPlayerController::OnRoundBegin);
    gameMode->OnRoundEnd().AddUObject(this, &AMainPlayerController::OnRoundEnd);
    gameMode->OnGameWon().AddUObject(this, &AMainPlayerController::OnGameWon);
  }
}

void AMainPlayerController::SetupInputComponent()
{
  Super::SetupInputComponent();

  InputComponent->BindAxis(TEXT("MoveRight"), this, &AMainPlayerController::OnInput_MoveRight);
  InputComponent->BindAxis(TEXT("MoveUp"), this, &AMainPlayerController::OnInput_MoveUp);
  InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMainPlayerController::OnInput_Attack);
  InputComponent->BindAction(TEXT("QuitGame"), EInputEvent::IE_Pressed, this, &AMainPlayerController::OnInput_QuitGame);
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

void AMainPlayerController::OnInput_QuitGame()
{
  UE_LOG(LogTemp, Log, TEXT("Bye!"));
  FGenericPlatformMisc::RequestExit(false);
}

void AMainPlayerController::OnPlayerKilled()
{
  UE_LOG(LogTemp, Log, TEXT("Game Over"));
  ShowGameOverUI();

  AFollowersGameMode* gameMode = GetWorld()->GetAuthGameMode<AFollowersGameMode>();
  if(gameMode)
  {
    gameMode->StopGameplay();
  }
}

void AMainPlayerController::OnGameStart()
{
  if(GameOverWidget)
  {
    GameOverWidget->RemoveFromViewport();
    GameOverWidget = nullptr;
  }

  if(MainPlayerPawn)
  {
    MainPlayerPawn->Reincarnate();
    FInputModeGameOnly newInputMode;
    SetInputMode(newInputMode);
  }

  bShowMouseCursor = false;
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
  if(RoundResultWidgetClass && !GameOverWidget)
  {
    RoundResultWidget = CreateWidget<URoundResultWidget>(this, RoundResultWidgetClass);
    if(RoundResultWidget)
    {
      RoundResultWidget->UpdateCultsDisplay();
      RoundResultWidget->AddToViewport();
    }
  }
}

void AMainPlayerController::OnGameWon()
{
  ShowGameOverUI();
  if(GameOverWidget)
  {
    GameOverWidget->SetAsWon();
  }
}

void AMainPlayerController::ShowGameOverUI()
{
  UE_LOG(LogTemp, Log, TEXT("ShowGameOverUI"));
  if(GameOverWidgetClass)
  {
    GameOverWidget = CreateWidget<UGameOverWidget>(this, GameOverWidgetClass);
    if(GameOverWidget)
    {
      GameOverWidget->AddToViewport();

      FInputModeGameAndUI newInputMode;
      SetInputMode(newInputMode);

      bShowMouseCursor = true;
    }
  }
}