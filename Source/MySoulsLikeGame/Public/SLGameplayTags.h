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

	//Primary Attributes
	FGameplayTag Attributes_Primary_Vigor;
	FGameplayTag Attributes_Primary_Energy;
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Endurance;
	FGameplayTag Attributes_Primary_Luck;
	//Secondary Attributes
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxStamina;
	FGameplayTag Attributes_Secondary_MaxMentalStrength;
	FGameplayTag Attributes_Secondary_AttackPower;
	FGameplayTag Attributes_Secondary_Defense;
	FGameplayTag Attributes_Secondary_StaminaRegeneration;

protected:

private:
	static FSLGameplayTags GameplayTags;
};
