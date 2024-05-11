// copyright aaaasun


#include "Character/SLCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "SLGameplayTags.h"
#include "AbilitySystem/SLAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

ASLCharacterBase::ASLCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Bow = CreateDefaultSubobject<USkeletalMeshComponent>("Bow");
	Bow->SetupAttachment(GetMesh(), FName("BowSocket"));
	Bow->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ArrowPouch = CreateDefaultSubobject<USkeletalMeshComponent>("ArrowPouch");
	ArrowPouch->SetupAttachment(GetMesh(), FName("BowArrowSocket"));
	ArrowPouch->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ASLCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* ASLCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ASLCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	Bow->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

FVector ASLCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	//基于蒙太奇返回正确的插槽
	const FSLGameplayTags& GameplayTags = FSLGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Bow) && IsValid(Bow))
	{
		return Bow->GetSocketLocation(BowTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	return FVector();
}

void ASLCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Bow->SetSimulatePhysics(true);
	Bow->SetEnableGravity(true);
	Bow->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bDead = true;
}

void ASLCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

bool ASLCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ASLCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> ASLCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

void ASLCharacterBase::InitAbilityActorInfo()
{
}

void ASLCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ASLCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ASLCharacterBase::AddCharacterAbilities()
{
	USLAbilitySystemComponent* SLASC = CastChecked<USLAbilitySystemComponent>(AbilitySystemComponent);
	//检查权限
	if (!HasAuthority()) return;
	SLASC->AddCharacterAbilities(StartupAbilities);
}
