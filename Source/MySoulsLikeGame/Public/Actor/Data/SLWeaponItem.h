// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "SLAssetManager.h"
#include "Actor/Data/SLItem.h"
#include "SLWeaponItem.generated.h"

UENUM(BlueprintType)
enum class EWeaponClass : uint8
{
	Melee,
	Ranged
};

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

	UPROPERTY(EditDefaultsOnly, Category="Weapon Class Defaults")
	EWeaponClass WeaponClass = EWeaponClass::Melee;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AActor> WeaponActor;
};
