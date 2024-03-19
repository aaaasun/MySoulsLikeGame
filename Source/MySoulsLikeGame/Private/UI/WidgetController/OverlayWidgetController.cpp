// copyright asun


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/SLAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const USLAttributeSet* SLAttributeSet = CastChecked<USLAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(SLAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(SLAttributeSet->GetMaxHealth());
	OnStaminaChanged.Broadcast(SLAttributeSet->GetStamina());
	OnMaxStaminaChanged.Broadcast(SLAttributeSet->GetMaxStamina());
	OnMentalStrengthChanged.Broadcast(SLAttributeSet->GetMentalStrength());
	OnMaxMentalStrengthChanged.Broadcast(SLAttributeSet->GetMaxMentalStrength());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const USLAttributeSet* SLAttributeSet = CastChecked<USLAttributeSet>(AttributeSet);
	//ASC自带代理来回应当属性变化,返回的是代理,代理名FOnGameplayAttributeValueChange
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data /*FOnAttributeChangeData是属性更改时使用的临时参数结构*/)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnStaminaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetMaxStaminaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxStaminaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetMentalStrengthAttribute()).
	                        AddLambda(
		                        [this](const FOnAttributeChangeData& Data)
		                        {
			                        OnMentalStrengthChanged.Broadcast(Data.NewValue);
		                        }
	                        );

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetMaxMentalStrengthAttribute()).
	                        AddLambda(
		                        [this](const FOnAttributeChangeData& Data)
		                        {
			                        OnMaxMentalStrengthChanged.Broadcast(Data.NewValue);
		                        }
	                        );
}
