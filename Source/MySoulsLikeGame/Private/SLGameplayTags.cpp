// copyright aaaasun


#include "SLGameplayTags.h"
#include "GameplayTagsManager.h"

FSLGameplayTags FSLGameplayTags::GameplayTags;

void FSLGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"), FString("活力"));

	GameplayTags.Attributes_Primary_Energy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Energy"), FString("精力"));

	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"), FString("力量"));

	GameplayTags.Attributes_Primary_Endurance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Endurance"), FString("耐力"));

	GameplayTags.Attributes_Primary_Luck = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Luck"), FString("运气"));

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"), FString("最大生命值"));

	GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxStamina"), FString("最大体力"));

	GameplayTags.Attributes_Secondary_MaxMentalStrength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMentalStrength"), FString("最大心力"));

	GameplayTags.Attributes_Secondary_AttackPower = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.AttackPower"), FString("攻击力"));

	GameplayTags.Attributes_Secondary_Defense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Defense"), FString("防御力"));

	GameplayTags.Attributes_Secondary_StaminaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.StaminaRegeneration"), FString("体力恢复"));
}
