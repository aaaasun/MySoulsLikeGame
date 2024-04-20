// copyright aaaasun


#include "AbilitySystem/Abilities/SLProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
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
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
			DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		Projectile->DamageEffectSpecHandle = SpecHandle;

		//step2
		Projectile->FinishSpawning(SpwanTransform);
	}
}
