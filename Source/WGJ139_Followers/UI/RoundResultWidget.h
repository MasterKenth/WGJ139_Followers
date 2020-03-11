// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class WGJ139_FOLLOWERS_API URoundResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCultEntryWidget> CultEntryWidgetTemplate;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* RoundNumberText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UVerticalBox* CultsContainer;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* NextRoundTimeText;

	void UpdateCultsDisplay();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
};
