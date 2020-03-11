// © Master Kenth 2020


#include "FollowersGameMode.h"
#include "../Gameplay/SpawnVolume.h"
#include "../Gameplay/NPCPawn.h"
#include "FollowersGameState.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"
#include "BasePawn.h"
#include "GameFramework/PlayerController.h"

DEFINE_LOG_CATEGORY(LogFollowersGameMode);

void AFollowersGameMode::AddFollowerForNextRound(int32 CultID, int32 Add)
{
  if(FollowersGameState)
  {
    FCultData* cult = FollowersGameState->Cults.FindByPredicate([CultID](FCultData& cult) { return cult.ID == CultID; });
    if(cult)
    {
      cult->Followers += Add;
    }
    else
    {
      UE_LOG(LogFollowersGameMode, Warning, TEXT("No cult with id %d"), CultID);
    }
    
    if(!FollowersGameState->bWonGame)
    {
      int32 playerCultID = FollowersGameState->Cults[0].ID;
      bool bAnyEnemyCultLeft = FollowersGameState->Cults.ContainsByPredicate([playerCultID](const FCultData& cult){ return cult.Followers > 0 && cult.ID != playerCultID; });
      
      if(!bAnyEnemyCultLeft)
      {
        FollowersGameState->bWonGame = true;
        StopGameplay();
        GameWonEvent.Broadcast();
      }
    }
  }
}

void AFollowersGameMode::StopGameplay()
{
  if(FollowersGameState)
  {
    GetWorldTimerManager().ClearTimer(FollowersGameState->RoundEndHandle);
    GetWorldTimerManager().ClearTimer(FollowersGameState->NextRoundBeginHandle);
  }
}

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
      for(ANPCPawn* pawn : FollowersGameState->SpawnedFollowers)
      {
        if(pawn)
        {
          pawn->Destroy();
        }
      }
      FollowersGameState->SpawnedFollowers.Empty();
      FollowersGameState->CurrentRound = 0;
      FollowersGameState->bWonGame = false;

      SetupCults();

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
      FCultData& newCult = FollowersGameState->Cults.Add_GetRef(GeneratePseudoRandomCult(FollowersGameState->Cults));
      newCult.Followers = 1;

      ABasePawn* playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn<ABasePawn>();
      if(playerPawn)
      {
        playerPawn->CultID = newCult.ID;
        playerPawn->DynamicMaterialInstance->SetVectorParameterValue(TEXT("Color"), newCult.Color);
      }
    }

    // Generate NPC cults
    const int32 totalFollowersToGenerate = 99;
    const int32 numNPCCults = FMath::RandRange(2, 5);
    int32 followersGenerated = 0;
    for(int32 i = 0; i < numNPCCults; i++)
    {
      const int32 followersLeft = totalFollowersToGenerate - followersGenerated;
      const int32 cultsLeftToGenerate = numNPCCults - i;
      int32 newFollowers;

      if(cultsLeftToGenerate == 1)
      {
        newFollowers = followersLeft;
      }
      else
      {
        const int32 averageFollowersLeft = FMath::RoundToInt(followersLeft / cultsLeftToGenerate);
        newFollowers = FMath::RoundToInt(averageFollowersLeft * FMath::RandRange(0.75f, 1.25f));
        newFollowers = FMath::Clamp(newFollowers, 1, followersLeft - cultsLeftToGenerate);
      }

      FCultData& newCult = FollowersGameState->Cults.Add_GetRef(GeneratePseudoRandomCult(FollowersGameState->Cults));
      newCult.Followers = newFollowers;

      followersGenerated += newFollowers;
    }
  }
}

void AFollowersGameMode::BeginRound()
{
  if(FollowersGameState)
  {
    UE_LOG(LogFollowersGameMode, Log, TEXT("BeginRound %d"), FollowersGameState->CurrentRound);

    // Reset player
    ABasePawn* playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn<ABasePawn>();
    if(playerPawn)
    {
      playerPawn->SetHealth(playerPawn->GetMaxHealth());
      playerPawn->SetActorLocation(FVector::ZeroVector);
    }

    // Spawn NPCs
    if(FollowersGameState->ArenaSpawnVolume && FollowersGameState->NPCPawnClass)
    {
      const FBoxSphereBounds bounds = FollowersGameState->ArenaSpawnVolume->GetBounds();
      
      for(int32 c = 0; c < FollowersGameState->Cults.Num(); c++)
      {
        FCultData& cult = FollowersGameState->Cults[c];
        UE_LOG(LogFollowersGameMode, Log, TEXT("round=#%d cult=%d f=%d (last: %d) %s"), FollowersGameState->CurrentRound, cult.ID, cult.Followers, cult.LastRoundFollowerCount, *cult.Name.ToString());

        cult.LastRoundFollowerCount = cult.Followers;

        const int32 followersToGenerate = (c == 0) ? cult.Followers - 1 : cult.Followers; // Skip first follower of cult #0 (the player)
        
        for(int32 i = 0; i < followersToGenerate; i++)
        {
          ANPCPawn* newPawn = GetWorld()->SpawnActor<ANPCPawn>(FollowersGameState->NPCPawnClass);
          if(newPawn)
          {
            FollowersGameState->SpawnedFollowers.Add(newPawn);
            newPawn->SpawnDefaultController();
            newPawn->CultID = cult.ID;

            auto mid = UMaterialInstanceDynamic::Create(FollowersGameState->CultNPCPawnMaterialBase, (UObject*)this);
            if(mid)
            {
              mid->SetVectorParameterValue(TEXT("Color"), cult.Color);
              newPawn->SetMaterial(mid);
            }

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

    GetWorldTimerManager().SetTimer(FollowersGameState->RoundEndHandle, this, &AFollowersGameMode::EndRound, 20.0f, false);
  }

  RoundBeginEvent.Broadcast();
}

void AFollowersGameMode::EndRound()
{
  if(FollowersGameState)
  {
    UE_LOG(LogFollowersGameMode, Log, TEXT("EndRound %d"), FollowersGameState->CurrentRound);
    FollowersGameState->CurrentRound++;

    for(ABasePawn* pawn : FollowersGameState->SpawnedFollowers)
    {
      if(pawn != nullptr && pawn->IsValidLowLevel())
      {
        pawn->Destroy();
      }
    }
    FollowersGameState->SpawnedFollowers.Empty();

    GetWorldTimerManager().SetTimer(FollowersGameState->NextRoundBeginHandle, this, &AFollowersGameMode::BeginRound, 5.0f, false);
  }

  RoundEndEvent.Broadcast();
}

FCultData AFollowersGameMode::GeneratePseudoRandomCult(const TArray<FCultData>& AlreadyGeneratedCults) const
{
  static int32 globalCultID = 0;
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

  static auto tempColors = TArray<FLinearColor>({
        FLinearColor::White,
        FLinearColor(0.3f, 0.3f, 0.3f), // Dark Gray
        FLinearColor(0.05f, 0.05f, 0.05f), // Black 
        FLinearColor::Red,
        FLinearColor::Green,
        FLinearColor::Blue,
        FLinearColor::Yellow,
        FLinearColor(FColor::Cyan),
        FLinearColor(0.7f, 0.4f, 0.7f), // Pink
        FLinearColor(0.4f, 0.3, 0.04f), // Dark Orange / Gold
        FLinearColor(0.2f, 0.12, 0.08f), // Brown
        FLinearColor(FColor::Purple),
        FLinearColor(FColor::Emerald)
  });

  FCultData newCult;
  newCult.ID = globalCultID++;
  newCult.Followers = 0;

  // Generate unique cult name
  int32 tries = 0;
  int32 maxTries = tempCoolNames.Num();

  int32 nameIndex = FMath::RandRange(0, tempCoolNames.Num() - 1);
  FString chosenName = tempCoolNames[nameIndex];

  while(tries <= maxTries && AlreadyGeneratedCults.ContainsByPredicate([chosenName] (const FCultData& cult) { return cult.Name.ToString() == chosenName; }))
  {
    nameIndex = (nameIndex + 1) % tempCoolNames.Num();
    chosenName = tempCoolNames[nameIndex];
    tries++;
  };

  if(tries > maxTries)
  {
    UE_LOG(LogFollowersGameMode, Warning, TEXT("Unable to uniquely name new cult"));
  }

  newCult.Name = FText::FromString(tempCoolNames[nameIndex]);
  newCult.LastRoundFollowerCount = 0;

  // Generate unique random color
  tries = 0;
  maxTries = tempColors.Num();

  int32 colorIndex = FMath::RandRange(0, tempColors.Num() - 1);
  FLinearColor chosenColor = tempColors[colorIndex];

  while(tries <= maxTries && AlreadyGeneratedCults.ContainsByPredicate([chosenColor] (const FCultData& cult) { return cult.Color == chosenColor; }))
  {
    colorIndex = (colorIndex + 1) % tempColors.Num();
    chosenColor = tempColors[colorIndex];
    tries++;
  };

  if(tries > maxTries)
  {
    UE_LOG(LogFollowersGameMode, Warning, TEXT("Unable to uniquely set cult color"));
  }

  newCult.Color = chosenColor;

  return newCult;
}