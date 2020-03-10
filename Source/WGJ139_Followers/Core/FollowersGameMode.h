// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FollowersGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFollowersGameMode, Log, All);
DECLARE_EVENT(AFollowersGameMode, FGameStartEvent);

UCLASS()
class WGJ139_FOLLOWERS_API AFollowersGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	FGameStartEvent& OnGameStart() { return GameStartEvent; }
	void AddFollowerForNextRound(int32 CultID, int32 Add);
	
protected:
	UPROPERTY()
	class AFollowersGameState* FollowersGameState;

	FGameStartEvent GameStartEvent;

	virtual void BeginPlay() override;

	void StartGame();
	void SetupCults();
	void BeginRound();
	void EndRound();

	struct FCultData GeneratePseudoRandomCult(const TArray<FCultData>& AlreadyGeneratedCults) const;
};
