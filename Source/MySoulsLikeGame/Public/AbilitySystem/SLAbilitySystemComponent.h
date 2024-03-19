// copyright asun

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SLAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API USLAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	
protected:
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
	                   FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
