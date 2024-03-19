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

protected:
	virtual void StartInitialLoading() override;
};
