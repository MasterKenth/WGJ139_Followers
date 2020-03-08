// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCPawn.h"

ANPCPawn::ANPCPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANPCPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANPCPawn::BeginPlay()
{
	Super::BeginPlay();
}