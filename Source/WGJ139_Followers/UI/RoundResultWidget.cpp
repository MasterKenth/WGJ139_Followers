// © Master Kenth 2020


#include "RoundResultWidget.h"
#include "../Core/FollowersGameState.h"
#include "CultEntryWidget.h"
#include "Engine/World.h"
#include "Components/VerticalBox.h"
#include "TimerManager.h"
#include "Components/TextBlock.h"

void URoundResultWidget::UpdateCultsDisplay()
{
  const AFollowersGameState* gameState = GetWorld()->GetGameState<AFollowersGameState>();
  if(RoundNumberText)
  {
    if(gameState)
    {
      RoundNumberText->SetText(FText::AsNumber(gameState->CurrentRound)); // Note: round has already been incremented at this point
    }
  }

  if(CultsContainer)
  {
    CultsContainer->ClearChildren();

    if(gameState && CultEntryWidgetTemplate)
    {
      for(int32 i = 0; i < gameState->Cults.Num(); i++)
      {
        const FCultData& cult = gameState->Cults[i];
        UCultEntryWidget* cultEntry = CreateWidget<UCultEntryWidget>(this, CultEntryWidgetTemplate);
        if(cultEntry)
        {
          FText nameText = cult.Name;
          if(i == 0)
          {
            nameText = FText::FromString(FString::Printf(TEXT("(you) %s"), *cult.Name.ToString()));
          }
          cultEntry->SetData(nameText, cult.Color, cult.Followers, cult.Followers - cult.LastRoundFollowerCount);
          CultsContainer->AddChild(cultEntry);
        }
      }
    }
  }
}

void URoundResultWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
  Super::NativeTick(MyGeometry, InDeltaTime);

  if(NextRoundTimeText)
  {
    const AFollowersGameState* gameState = GetWorld()->GetGameState<AFollowersGameState>();
    if(gameState)
    {
      const float timeToNextRound = GetWorld()->GetTimerManager().GetTimerRemaining(gameState->NextRoundBeginHandle);
      const int32 timeToNextRoundFullSeconds = FMath::CeilToInt(timeToNextRound);
      NextRoundTimeText->SetText(FText::AsNumber(timeToNextRoundFullSeconds));
    }
  }
}