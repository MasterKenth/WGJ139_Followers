// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BrushComponent.h"

ASpawnVolume::ASpawnVolume()
{
  GetBrushComponent()->SetCollisionProfileName(TEXT("NoCollision"));
	bColored = true;
	BrushColor.R = 0;
	BrushColor.G = 255;
	BrushColor.B = 100;
	BrushColor.A = 255;
}