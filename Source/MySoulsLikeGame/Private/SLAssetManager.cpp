// copyright aaaasun


#include "SLAssetManager.h"
#include "SLGameplayTags.h"

USLAssetManager& USLAssetManager::Get()
{
	check(GEngine);
	USLAssetManager* SLAssetManager = Cast<USLAssetManager>(GEngine->AssetManager);
	return *SLAssetManager;
}

//开始加载游戏资产时
void USLAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FSLGameplayTags::InitializeNativeGameplayTags();
}
