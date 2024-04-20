// copyright aaaasun


#include "AbilitySystem/Abilities/SLMeleeAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "SLGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"

void USLMeleeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	// if (!bIsServer) return;
	//
	// const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
	// 	GetAvatarActorFromActorInfo());
	// const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(),
	//                                                                          SourceASC->MakeEffectContext());
	// const FSLGameplayTags GameplayTags = FSLGameplayTags::Get();
	// const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	// UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);
}
