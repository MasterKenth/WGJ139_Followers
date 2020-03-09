// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowersGameMode.h"
#include "../Gameplay/SpawnVolume.h"
#include "../Gameplay/NPCPawn.h"
#include "FollowersGameState.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY(LogFollowersGameMode);

void AFollowersGameMode::BeginPlay()
{
  Super::BeginPlay();

  FollowersGameState = Cast<AFollowersGameState>(GameState);

  if(FollowersGameState)
  {
    FollowersGameState->ArenaSpawnVolume = nullptr;
    for(TActorIterator<ASpawnVolume> itr(GetWorld()); itr; ++itr)
    {
      FollowersGameState->ArenaSpawnVolume = *itr;
      break;
    }

    if(!FollowersGameState->ArenaSpawnVolume)
    {
      UE_LOG(LogFollowersGameMode, Error, TEXT("No spawn volume found"));
    }
  }

  FTimerHandle handle;
  GetWorldTimerManager().SetTimer(handle, this, &AFollowersGameMode::StartGame, 2.0f);
}

void AFollowersGameMode::StartGame()
{
    UE_LOG(LogFollowersGameMode, Log, TEXT("StartGame"));

    if(FollowersGameState)
    {
      SetupCults();
      FollowersGameState->CurrentRound = 0;

      GameStartEvent.Broadcast();

      BeginRound();
    }
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

void AFollowersGameMode::BeginRound()
{
  if(FollowersGameState)
  {
    UE_LOG(LogFollowersGameMode, Log, TEXT("BeginRound %d"), FollowersGameState->CurrentRound);

    // Spawn NPCs
    if(FollowersGameState->ArenaSpawnVolume && FollowersGameState->NPCPawnClass)
    {
      const FBoxSphereBounds bounds = FollowersGameState->ArenaSpawnVolume->GetBounds();
      
      for(const FCultData& cult : FollowersGameState->Cults)
      {
        for(int32 i = 0; i < cult.Followers; i++)
        {
          ANPCPawn* newPawn = GetWorld()->SpawnActor<ANPCPawn>(FollowersGameState->NPCPawnClass);
          if(newPawn)
          {
            FollowersGameState->SpawnedFollowers.Add(newPawn);

            FVector newLoc;
            FHitResult hit;
            int32 spawnTries = 0;
            const int32 maxSpawnTries = 100;
            do
            {
              if(hit.Actor.IsValid())
              {
                UE_LOG(LogFollowersGameMode, Warning, TEXT("Spawn loc sweep hit %s at loc %s"), *hit.GetActor()->GetName(), *newLoc.ToCompactString());
              }
              newLoc = FMath::RandPointInBox(bounds.GetBox());
              newLoc.Z = 0;
              newPawn->SetActorLocation(newLoc, true, &hit, ETeleportType::ResetPhysics);
              spawnTries++;
              if(spawnTries >= maxSpawnTries && hit.bBlockingHit)
              {
                UE_LOG(LogFollowersGameMode, Warning, TEXT("Unable to find suitable spawn location for NPC pawn"));
              }
            }
            while(spawnTries < maxSpawnTries && hit.bBlockingHit);
          }
          else
          {
            UE_LOG(LogFollowersGameMode, Error, TEXT("Unable to spawn NPC pawn"));
            break;
          }
        }
      }
    }
  }
}

void AFollowersGameMode::EndRound()
{

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