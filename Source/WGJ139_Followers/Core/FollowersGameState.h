// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FollowersGameState.generated.h"

USTRUCT()
struct WGJ139_FOLLOWERS_API FCultData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FText Name;

	UPROPERTY()
	int32 Followers;
};

UCLASS()
class WGJ139_FOLLOWERS_API AFollowersGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FCultData> Cults;
};
