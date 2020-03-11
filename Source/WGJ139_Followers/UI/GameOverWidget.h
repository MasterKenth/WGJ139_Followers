// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class WGJ139_FOLLOWERS_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UTextBlock*> FollowerCountTexts;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UHorizontalBox* FailedTextContainer;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UHorizontalBox* OneFollowerTextContainer;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UHorizontalBox* ManyFollowersTextContainer;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* RetryButton;

protected:
	virtual void NativeConstruct() override;
};
