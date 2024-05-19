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

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	

protected:
	//使其成为RPC
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec,
	                         FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
