// © Master Kenth 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FollowersGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFollowersGameMode, Log, All);
DECLARE_EVENT(AFollowersGameMode, FGameStartEvent);
DECLARE_EVENT(AFollowersGameMode, FRoundBeginEvent);
DECLARE_EVENT(AFollowersGameMode, FRoundEndEvent);
DECLARE_EVENT(AFollowersGameMode, FGameWonEvent);

UCLASS()
class WGJ139_FOLLOWERS_API AFollowersGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	FGameStartEvent& OnGameStart() { return GameStartEvent; }
	FRoundBeginEvent& OnRoundBegin() { return RoundBeginEvent; }
	FRoundEndEvent& OnRoundEnd() { return RoundEndEvent; }
	FGameWonEvent& OnGameWon() { return GameWonEvent; }
	void AddFollowerForNextRound(int32 CultID, int32 Add);

	void StartGame();
	void StopGameplay();
	
protected:
	UPROPERTY()
	class AFollowersGameState* FollowersGameState;

	FGameStartEvent GameStartEvent;
	FRoundBeginEvent RoundBeginEvent;
	FRoundEndEvent RoundEndEvent;
	FGameWonEvent GameWonEvent;

	virtual void BeginPlay() override;

	void SetupCults();
	void BeginRound();
	void EndRound();

	struct FCultData GeneratePseudoRandomCult(const TArray<FCultData>& AlreadyGeneratedCults) const;
};
