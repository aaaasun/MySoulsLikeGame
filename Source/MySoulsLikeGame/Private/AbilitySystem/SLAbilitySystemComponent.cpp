// copyright asun


#include "AbilitySystem/SLAbilitySystemComponent.h"

#include "SLGameplayTags.h"

//函数调用时我们知道AbilityActorInfo被设置好了
void USLAbilitySystemComponent::AbilityActorInfoSet()
{
	//OnGameplayEffectAppliedDelegateToSelf:应用效果时的委托
	//每当将 GE 应用于自身时，都会在服务器上调用。这包括即时和基于持续时间的 GE。
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &USLAbilitySystemComponent::EffectApplied);
}

void USLAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                              const FGameplayEffectSpec& EffectSpec,
                                              FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
}
