// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FollowersGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFollowersGameMode, Log, All);

UCLASS()
class WGJ139_FOLLOWERS_API AFollowersGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
protected:
	UPROPERTY()
	class AFollowersGameState* FollowersGameState;

	virtual void BeginPlay() override;

	void SetupCults();

	struct FCultData GenerateRandomCult() const;
};
