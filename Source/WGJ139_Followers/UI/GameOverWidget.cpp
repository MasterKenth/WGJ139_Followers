// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "../Core/FollowersGameState.h"
#include "Engine/World.h"
#include "Components/HorizontalBox.h"
#include "../Core/FollowersGameMode.h"

void UGameOverWidget::NativeConstruct()
{
  AFollowersGameState* gameState = GetWorld()->GetGameState<AFollowersGameState>();
  if(gameState)
  {
    int32 followers = gameState->Cults[0].Followers - 1;
    for(UTextBlock* countText : FollowerCountTexts)
    {
      countText->SetText(FText::AsNumber(followers));
    }

  if(FailedTextContainer && OneFollowerTextContainer && ManyFollowersTextContainer)
  {
    FailedTextContainer->SetVisibility(ESlateVisibility::Collapsed);
    OneFollowerTextContainer->SetVisibility(ESlateVisibility::Collapsed);
    ManyFollowersTextContainer->SetVisibility(ESlateVisibility::Collapsed);
    
    if(followers == 0)
    {
      FailedTextContainer->SetVisibility(ESlateVisibility::Visible);
    }
    else if(followers == 1)
    {
      OneFollowerTextContainer->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
      ManyFollowersTextContainer->SetVisibility(ESlateVisibility::Visible);
    }
  }

  }
  if(RetryButton)
  {
    RetryButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnRetryClicked);
  }
}

void UGameOverWidget::OnRetryClicked()
{
  AFollowersGameMode* gameMode = GetWorld()->GetAuthGameMode<AFollowersGameMode>();
  if(gameMode)
  {
    gameMode->StartGame();
  }
}