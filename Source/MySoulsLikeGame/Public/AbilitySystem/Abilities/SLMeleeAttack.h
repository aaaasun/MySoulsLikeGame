// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SLDamageGameplayAbility.h"
#include "SLMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API USLMeleeAttack : public USLDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetDamageEffectSpecHandle();
};
