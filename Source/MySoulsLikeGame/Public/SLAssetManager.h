// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SLAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API USLAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static USLAssetManager& Get();

	/** 物品的静态类型 */
	static const FPrimaryAssetType	PotionItemType;
	static const FPrimaryAssetType	WeaponItemType;

protected:
	virtual void StartInitialLoading() override;
};
