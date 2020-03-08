// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/BasePawn.h"
#include "NPCPawn.generated.h"

UCLASS()
class WGJ139_FOLLOWERS_API ANPCPawn : public ABasePawn
{
	GENERATED_BODY()

public:
	ANPCPawn();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
