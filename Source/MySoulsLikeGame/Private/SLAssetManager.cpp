// copyright aaaasun


#include "SLAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "SLGameplayTags.h"

const FPrimaryAssetType USLAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType USLAssetManager::WeaponItemType = TEXT("Weapon");

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
	//要使用TargetData这里必须要加
	UAbilitySystemGlobals::Get().InitGlobalData();
}
