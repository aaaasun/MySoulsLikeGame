// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "SLAssetManager.h"
#include "Actor/Data/SLItem.h"
#include "SLWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API USLWeaponItem : public USLItem
{
	GENERATED_BODY()

public:
	USLWeaponItem()
	{
		ItemType = USLAssetManager::WeaponItemType;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AActor> WeaponActor;
};
