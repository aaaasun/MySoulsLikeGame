// copyright aaaasun


#include "AbilitySystem/Abilities/SLMeleeAttack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "SLGameplayTags.h"
#include "Actor/SLBaseWeapon.h"

void USLMeleeAttack::SetDamageEffectSpecHandle()
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	//给武器添加GESpec来造成伤害
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
		GetAvatarActorFromActorInfo());

	ASLBaseWeapon* MeleeWeapon = ICombatInterface::Execute_GetCombatWeapon(
		GetAvatarActorFromActorInfo(), FSLGameplayTags::Get().Montage_Attack_Weapon);
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	//设置好EffectContext的属性
	EffectContextHandle.SetAbility(this); //设置AbilityCDO、AbilityLevel和AbilityInstanceNotReplicated
	EffectContextHandle.AddSourceObject(MeleeWeapon); //设置SourceObject
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(MeleeWeapon);
	EffectContextHandle.AddActors(Actors); //设置Actors
	FHitResult HitResult;
	EffectContextHandle.AddHitResult(HitResult); //设置HitResult
	
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
		DamageEffectClass,
		GetAbilityLevel(),
		EffectContextHandle);
	FSLGameplayTags GameplayTags = FSLGameplayTags::Get();
	
	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}
	
	MeleeWeapon->DamageEffectSpecHandle = SpecHandle;
}
