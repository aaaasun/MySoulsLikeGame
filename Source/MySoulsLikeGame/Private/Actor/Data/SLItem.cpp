// copyright aaaasun


#include "Actor/Data/SLItem.h"

 bool USLItem::IsConsumable() const
 {
 	if (MaxCount <= 0)
 	{
 		return true;
 	}
 	return false;
 }

 FString USLItem::GetIdentifierString() const
 {
 	return GetPrimaryAssetId().ToString();
 }

FPrimaryAssetId USLItem::GetPrimaryAssetId() const
{
 	return FPrimaryAssetId(ItemType, GetFName());
}
