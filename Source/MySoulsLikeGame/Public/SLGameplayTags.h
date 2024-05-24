// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *SLGameplayTags
 *
 * 单例含原生Tags
 */

struct FSLGameplayTags
{
public:
	static const FSLGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_Energy;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Endurance;
	FGameplayTag Attributes_Primary_Luck;

	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxStamina;
	FGameplayTag Attributes_Secondary_MaxMentalStrength;
	FGameplayTag Attributes_Secondary_AttackPower;
	FGameplayTag Attributes_Secondary_Defense;
	FGameplayTag Attributes_Secondary_StaminaRegeneration;

	FGameplayTag Input_LMB;
	FGameplayTag Input_RMB;
	FGameplayTag Input_1;
	FGameplayTag Input_C;
	FGameplayTag Input_E;
	FGameplayTag Input_Space;

	FGameplayTag Damage;
	FGameplayTag Damage_Physical;
	FGameplayTag Damage_Fire;

	FGameplayTag Attributes_Resistance_Physical;
	FGameplayTag Attributes_Resistance_Fire;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

	FGameplayTag Abilities_Attack;

	FGameplayTag Montage_Attack_Weapon;
	FGameplayTag Montage_Attack_Bow;
	FGameplayTag Montage_Attack_RightHand;
	FGameplayTag Montage_Attack_LeftHand;

	FGameplayTag Effect_HitReact;

	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;

	FGameplayTag Weapon_Melee_Sword;
	FGameplayTag Weapon_Ranged_Bow;

private:
	static FSLGameplayTags GameplayTags;
};
