// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "SLAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API USLAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
