// copyright aaaasun


#include "Character/SLCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "SLGameplayTags.h"
#include "AbilitySystem/SLAbilitySystemComponent.h"
#include "Actor/SLBaseWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "MySoulsLikeGame/MySoulsLikeGame.h"
#include "Net/UnrealNetwork.h"

ASLCharacterBase::ASLCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);

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

void ASLCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASLCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASLCharacterBase, OverlappingWeapon, COND_OwnerOnly);
}

void ASLCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent)
	{
		CombatComponent->Character = this;
	}
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
	CombatComponent->EquippedMeleeWeapon->SetLifeSpan(CombatComponent->EquippedMeleeWeapon->GetLifeSpanAfterDeath());
	CombatComponent->EquippedRangedWeapon->SetLifeSpan(CombatComponent->EquippedRangedWeapon->GetLifeSpanAfterDeath());
	MulticastHandleDeath();
}

UCombatComponent* ASLCharacterBase::GetCombatComponent_Implementation()
{
	return CombatComponent;
}

ASLBaseWeapon* ASLCharacterBase::GetOverlapWeapon_Implementation()
{
	return OverlappingWeapon;
}

ASLBaseWeapon* ASLCharacterBase::GetCombatWeapon_Implementation(const FGameplayTag& MontageTag)
{
	//基于蒙太奇返回正确的插槽
	const FSLGameplayTags& GameplayTags = FSLGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Weapon) && IsValid(Weapon))
	{
		return CombatComponent->EquippedMeleeWeapon;
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Bow) && IsValid(Bow))
	{
		return CombatComponent->EquippedRangedWeapon;
	}
	return nullptr;
}

USkeletalMeshComponent* ASLCharacterBase::GetCharacterMesh_Implementation()
{
	return GetMesh();
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

bool ASLCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ASLCharacterBase::GetAvatar_Implementation()
{
	return this;
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

void ASLCharacterBase::OnRep_OverlappingWeapon(ASLBaseWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void ASLCharacterBase::SetOverlappingWeapon(ASLBaseWeapon* InWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = InWeapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void ASLCharacterBase::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InAbilities)
{
	//检查权限
	if (!HasAuthority()) return;
	USLAbilitySystemComponent* SLASC = CastChecked<USLAbilitySystemComponent>(AbilitySystemComponent);
	SLASC->AddCharacterAbilities(InAbilities);
}

void ASLCharacterBase::JumpSectionForCombo(const FName NextSection)
{
	if (EnableComboPeriod)
	{
		UAnimInstance* Anim = GetMesh()->GetAnimInstance();
		const UAnimMontage* CurMontage = Anim->GetCurrentActiveMontage();
		const FName CurSection = Anim->Montage_GetCurrentSection(CurMontage);
		Anim->Montage_SetNextSection(CurSection, NextSection, CurMontage);
		EnableComboPeriod = false;
	}
}
