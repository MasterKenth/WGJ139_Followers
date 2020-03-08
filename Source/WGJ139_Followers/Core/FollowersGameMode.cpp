// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowersGameMode.h"
#include "FollowersGameState.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY(LogFollowersGameMode);

void AFollowersGameMode::BeginPlay()
{
  Super::BeginPlay();

  FollowersGameState = Cast<AFollowersGameState>(GameState);

  //FTimerHandle handle;
  //GetWorldTimerManager().SetTimer(handle, this, &AFollowersGameMode::SetupCults, 5.0f, true, 5.0f);
}

void AFollowersGameMode::SetupCults()
{
  if(FollowersGameState)
  {
    FollowersGameState->Cults.Empty();

    // Add player cult
    {
      FCultData& newCult = FollowersGameState->Cults.Add_GetRef(GenerateRandomCult());
      newCult.Followers = 1;
    }

    // Generate NPC cults
    int32 totalFollowers = 99;
    int32 numNPCCults = FMath::RandRange(2, 5);
    int32 followersGenerated = 0;
    for(int32 i = 0; i < numNPCCults; i++)
    {
      int32 followersLeft = totalFollowers - followersGenerated;
      int32 cultsLeftToGenerate = numNPCCults - i;
      int32 newFollowers;

      if(cultsLeftToGenerate == 1)
      {
        newFollowers = followersLeft;
      }
      else
      {
        int32 averageFollowersLeft = FMath::RoundToInt(followersLeft / cultsLeftToGenerate);
        newFollowers = FMath::RoundToInt(averageFollowersLeft * FMath::RandRange(0.75f, 1.25f));
        newFollowers = FMath::Clamp(newFollowers, 1, followersLeft - cultsLeftToGenerate);
      }

      FCultData& newCult = FollowersGameState->Cults.Add_GetRef(GenerateRandomCult());
      newCult.Followers = newFollowers;

      followersGenerated += newFollowers;
    }

    UE_LOG(LogFollowersGameMode, Log, TEXT("-------------- SetupCults --------------"));
    UE_LOG(LogFollowersGameMode, Log, TEXT("Cults: %d"), FollowersGameState->Cults.Num());
    int32 accumTotalFollowers = 0;
    for(int32 i = 0; i < FollowersGameState->Cults.Num(); i++)
    {
      FCultData& cult = FollowersGameState->Cults[i];
      accumTotalFollowers += cult.Followers;
      UE_LOG(LogFollowersGameMode, Log, TEXT("\t#%d \t%d/%d followers \t%s\t"), i, cult.Followers, accumTotalFollowers, *cult.Name.ToString());
    }
  }
}

FCultData AFollowersGameMode::GenerateRandomCult() const
{
  static auto tempCoolNames = TArray<FString>({
    "Seth",
    "Malifucius",
    "Dan",
    "Truth",
    "Abraca",
    "Despairio",
    "Homeus",
    "Moha Purk",
    "Seecharp Bad",
    "the Unreal God",
    "Divinto",
    "Bacciatio",
    "Yare Yare Dase",
    "Poliorno",
    "Kira"
  });

  FCultData newCult;
  newCult.Name = FText::FromString(tempCoolNames[FMath::RandRange(0, tempCoolNames.Num() - 1)]);
  newCult.Followers = 0;
  return newCult;
}