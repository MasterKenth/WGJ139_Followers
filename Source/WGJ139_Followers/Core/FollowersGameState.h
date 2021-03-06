// © Master Kenth 2020

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
	int32 ID;

	UPROPERTY()
	FText Name;

	UPROPERTY()
	int32 Followers;

	UPROPERTY()
	int32 LastRoundFollowerCount;

	UPROPERTY()
	FLinearColor Color;
};

UCLASS()
class WGJ139_FOLLOWERS_API AFollowersGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ANPCPawn> NPCPawnClass;

	UPROPERTY(EditAnywhere)
	class UMaterialInterface* CultNPCPawnMaterialBase;

	UPROPERTY()
	TArray<FCultData> Cults;

	UPROPERTY()
	class ASpawnVolume* ArenaSpawnVolume;

	UPROPERTY()
	TArray<ANPCPawn*> SpawnedFollowers;

	UPROPERTY()
	int32 CurrentRound;

	UPROPERTY()
	FTimerHandle RoundEndHandle;

	UPROPERTY()
	FTimerHandle NextRoundBeginHandle;

	UPROPERTY()
	bool bWonGame;
};
