// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SLDamageGameplayAbility.h"
#include "SLMeleeAbility.generated.h"

class UGameplayEffect;

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API USLMeleeAbility : public USLDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
};
