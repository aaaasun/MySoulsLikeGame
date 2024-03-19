// copyright aaaasun


#include "AbilitySystem/Data/AttributeInfo.h"

#include "SLGameplayTags.h"

FSLAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FSLAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("找不到[%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FSLAttributeInfo();
}
