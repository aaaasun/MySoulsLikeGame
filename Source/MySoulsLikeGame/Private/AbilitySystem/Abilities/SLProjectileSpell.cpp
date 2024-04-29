// copyright aaaasun


#include "AbilitySystem/Abilities/SLProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "SLGameplayTags.h"
#include "Actor/SLProjectile.h"
#include "Interaction/CombatInterface.h"

void USLProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void USLProjectileSpell::SpawnProjectile(const FVector& TraceHitTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		const FVector HitTarget = TraceHitTarget;
		const FVector ToTarget = HitTarget - SocketLocation;
		const FRotator TargetRotation = ToTarget.Rotation();

		FTransform SpwanTransform;
		SpwanTransform.SetLocation(SocketLocation);
		SpwanTransform.SetRotation(TargetRotation.Quaternion());

		//SpawnActorDeferred:可以分两部来生成物体，以便在中间添加GE的信息
		//step1
		ASLProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASLProjectile>(
			ProjectileClass,
			SpwanTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//给飞行物添加GESpec来造成伤害
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
			GetAvatarActorFromActorInfo());

		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		//设置好EffectContext的属性
		EffectContextHandle.SetAbility(this); //设置AbilityCDO、AbilityLevel和AbilityInstanceNotReplicated
		EffectContextHandle.AddSourceObject(Projectile); //设置SourceObject
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EffectContextHandle.AddActors(Actors); //设置Actors
		//HitResult我在Projectile OnHit时设置

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

		Projectile->DamageEffectSpecHandle = SpecHandle;

		//step2
		Projectile->FinishSpawning(SpwanTransform);
	}
}
