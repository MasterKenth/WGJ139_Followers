// Fill out your copyright notice in the Description page of Project Settings.


#include "CultEntryWidget.h"
#include "Components/TextBlock.h"
#include "Styling/SlateColor.h"
#include "Math/Color.h"

void UCultEntryWidget::SetData(FText CultName, FLinearColor Color, int32 Followers, int32 FollowerDelta)
{
  if(CultNameText)
  {
    CultNameText->SetText(CultName);
    CultNameText->SetColorAndOpacity(FSlateColor(Color));
  }

  if(FollowerCountText)
  {
    FollowerCountText->SetText(FText::AsNumber(Followers));
  }

  if(FollowerDeltaText)
  {
    TCHAR prefixSign = (FollowerDelta > 0) ? '+' : ' ';
    FString str = FString::Printf(TEXT("%c%d"), prefixSign, FollowerDelta);
    FollowerDeltaText->SetText(FText::FromString(str));

    if(FollowerDelta > 0)
    {
      FollowerDeltaText->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
    }
    else if(FollowerDelta < 0)
    {
      FollowerDeltaText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
    }
    else
    {
      FollowerDeltaText->SetColorAndOpacity(FSlateColor(FLinearColor::Gray));
    }
  }
}
