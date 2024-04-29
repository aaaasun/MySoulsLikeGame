// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/SLDamageGameplayAbility.h"
#include "SLProjectileSpell.generated.h"

class ASLProjectile;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API USLProjectileSpell : public USLDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projecile")
	void SpawnProjectile(const FVector& TraceHitTarget);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASLProjectile> ProjectileClass;
};
