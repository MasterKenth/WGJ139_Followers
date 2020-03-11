// © Master Kenth 2020

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
	class UHorizontalBox* WonTextContainer;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* TitleText;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* RetryButtonText;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* RetryButton;

	void SetAsWon();

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnRetryClicked();
};
