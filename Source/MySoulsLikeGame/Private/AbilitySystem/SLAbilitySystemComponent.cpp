// copyright asun


#include "AbilitySystem/SLAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "SLGameplayTags.h"
#include "AbilitySystem/Abilities/SLGameplayAbility.h"
#include "Interaction/CombatInterface.h"

//函数调用时我们知道AbilityActorInfo被设置好了
void USLAbilitySystemComponent::AbilityActorInfoSet()
{
	//OnGameplayEffectAppliedDelegateToSelf:应用效果时的委托
	//每当将 GE 应用于自身时，都会在服务器上调用。这包括即时和基于持续时间的 GE。
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &USLAbilitySystemComponent::ClientEffectApplied);
}

void USLAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : InAbilities)
	{
		//如何赋予能力
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const USLGameplayAbility* SLAbility = Cast<USLGameplayAbility>(AbilitySpec.Ability))
		{
			//动态能力标签（DynamicAbilityTags）：可选择复制的能力标签，这些标签也会被应用的游戏效果捕获，作为源标签。
			AbilitySpec.DynamicAbilityTags.AddTag(SLAbility->AbilitiesInputTag);
			GiveAbility(AbilitySpec);
			if (SLAbility->bIsPassiveAbility)
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void USLAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
			else
			{
				FGameplayEventData Payload;
				Payload.Target = GetAvatarActor();
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
					GetAvatarActor(), FSLGameplayTags::Get().Combo_LightAttack, Payload);
			}
		}
	}
}

void USLAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//告知Ability Input已按下
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				//可能会有一些事件阻止该能力是实现，所以用TryActivateAbility
				// TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void USLAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//告知Ability Input已松开
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle,
			                      AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void USLAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                   const FGameplayEffectSpec& EffectSpec,
                                                                   FActiveGameplayEffectHandle
                                                                   ActiveGameplayEffectHandle)
{
}
