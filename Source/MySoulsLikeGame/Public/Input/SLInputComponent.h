// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "SLInputConfig.h"
#include "SLInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API USLInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserClass, typename PressFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const USLInputConfig* InputConfig, UserClass* Object, PressFuncType PressFunc,
	                        ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressFuncType, typename ReleasedFuncType, typename HeldFuncType>
void USLInputComponent::BindAbilityActions(const USLInputConfig* InputConfig, UserClass* Object,
                                           PressFuncType PressFunc,
                                           ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	for (const FSLInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressFunc, Action.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
