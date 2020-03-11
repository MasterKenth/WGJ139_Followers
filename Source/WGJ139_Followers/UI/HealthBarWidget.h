// © Master Kenth 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class WGJ139_FOLLOWERS_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* Bar;

	void SetHealthPercent(float Percent);

};
