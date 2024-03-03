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
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const USLAttributeSet* SLAttributeSet = CastChecked<USLAttributeSet>(AttributeSet);
	//ASC自带代理来回应当属性变化,返回的是代理,代理名FOnGameplayAttributeValueChange
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetHealthAttribute()).AddUObject(
		this, &UOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetMaxHealthAttribute()).AddUObject(
		this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetStaminaAttribute()).AddUObject(
		this, &UOverlayWidgetController::StaminaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetMaxStaminaAttribute()).
	                        AddUObject(this, &UOverlayWidgetController::MaxStaminaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::StaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnStaminaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxStaminaChanged.Broadcast(Data.NewValue);
}
