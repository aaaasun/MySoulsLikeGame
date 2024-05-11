// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SLGameplayAbility.h"
#include "SLDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API USLDamageGameplayAbility : public USLGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
