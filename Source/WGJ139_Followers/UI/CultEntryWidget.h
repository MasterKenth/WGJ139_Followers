// © Master Kenth 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CultEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class WGJ139_FOLLOWERS_API UCultEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* CultNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* FollowerCountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* FollowerDeltaText;

	void SetData(FText CultName, FLinearColor Color, int32 Followers, int32 FollowerDelta);
};
