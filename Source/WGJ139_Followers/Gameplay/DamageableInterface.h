// © Master Kenth 2020

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class WGJ139_FOLLOWERS_API IDamageableInterface
{
	GENERATED_BODY()

public:
	virtual void TakeDamage(int32 Damage, class AActor* DamageInstigator) = 0;
};
