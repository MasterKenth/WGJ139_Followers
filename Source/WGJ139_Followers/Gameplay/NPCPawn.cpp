// © Master Kenth 2020


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