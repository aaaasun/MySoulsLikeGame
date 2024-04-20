// copyright aaaasun


#include "Input/SLInputConfig.h"

const UInputAction* USLInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FSLInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("没找到%s"), *InputTag.ToString());
	}
	return nullptr;
}
