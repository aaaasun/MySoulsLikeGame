// copyright aaaasun


#include "SLGameplayTags.h"
#include "GameplayTagsManager.h"

FSLGameplayTags FSLGameplayTags::GameplayTags;

void FSLGameplayTags::InitializeNativeGameplayTags()
{
	/**
	 *Primary Attributes
	*/
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

	/**
	 *Secondary Attributes
	*/
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

	/**
	 *Input
	*/
	GameplayTags.Input_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.LMB"), FString("鼠标左键"));

	GameplayTags.Input_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.RMB"), FString("鼠标右键"));

	GameplayTags.Input_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.1"), FString("键盘数字1"));

	GameplayTags.Input_Alt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Input.Alt"), FString("键盘左Alt"));

	/**
	 *Damage
	*/
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"), FString("伤害"));

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"), FString("物理伤害类型"));

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"), FString("火焰伤害类型"));

	/**
	 *Resistances
	*/
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"), FString("物理伤害抵抗"));

	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"), FString("火焰伤害抵抗"));

	/**
	 *Map of Damage Types to Resistances
	*/
	GameplayTags.DamageTypesToResistances.Add(
		GameplayTags.Damage_Physical,
		GameplayTags.Attributes_Resistance_Physical
	);

	GameplayTags.DamageTypesToResistances.Add(
		GameplayTags.Damage_Fire,
		GameplayTags.Attributes_Resistance_Fire
	);

	/**
	 *Abilities
	*/
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"), FString("攻击"));

	/**
	 *Montgae
	*/
	GameplayTags.Montage_Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.Weapon"), FString("使用武器攻击"));

	GameplayTags.Montage_Attack_Bow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.Bow"), FString("使用弓箭攻击"));

	GameplayTags.Montage_Attack_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.RightHand"), FString("使用右手攻击"));

	GameplayTags.Montage_Attack_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.LeftHand"), FString("使用左手攻击"));
	
	/**
	 *Effect
	*/
	GameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effect.HitReact"), FString("中击反应"));

	/**
	 *Input Block
	*/
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputPressed"), FString("阻止按下输入"));

	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputHeld"), FString("阻止按住输入"));

	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputReleased"), FString("阻止松开输入"));
}
