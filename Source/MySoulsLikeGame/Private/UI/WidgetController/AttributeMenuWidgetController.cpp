// copyright aaaasun


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "SLGameplayTags.h"
#include "AbilitySystem/SLAttributeSet.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const USLAttributeSet* SLAttributeSet = CastChecked<USLAttributeSet>(AttributeSet);
	check(AttributeInfo);
	for (const TTuple<FGameplayTag, FGameplayAttribute(*)()>& Pair : SLAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const USLAttributeSet* SLAttributeSet = CastChecked<USLAttributeSet>(AttributeSet);
	check(AttributeInfo);
	for (const TTuple<FGameplayTag, FGameplayAttribute(*)()>& Pair : SLAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair, SLAttributeSet](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	FSLAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
