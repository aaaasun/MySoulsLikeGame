// copyright aaaasun


#include "AbilitySystem/SLAbilitySystemGlobals.h"

#include "SLAbilityTypes.h"

FGameplayEffectContext* USLAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FSLGameplayEffectContext;
}
