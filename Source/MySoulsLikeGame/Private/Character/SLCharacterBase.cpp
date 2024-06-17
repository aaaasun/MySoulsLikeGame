// copyright aaaasun


#include "Character/SLCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "SLGameplayTags.h"
#include "AbilitySystem/SLAbilitySystemComponent.h"
#include "Actor/SLBaseWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/CombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
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
}

void ASLCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASLCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASLCharacterBase, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME(ASLCharacterBase, bIsStaring);
	DOREPLIFETIME(ASLCharacterBase, bDead);
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
	if (CombatComponent->EquippedMeleeWeapon)
	{
		CombatComponent->EquippedMeleeWeapon->
		                 SetLifeSpan(CombatComponent->EquippedMeleeWeapon->GetLifeSpanAfterDeath());
	}
	if (CombatComponent->EquippedRangedWeapon)
	{
		CombatComponent->EquippedRangedWeapon->
		                 SetLifeSpan(CombatComponent->EquippedRangedWeapon->GetLifeSpanAfterDeath());
	}
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
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Weapon) && IsValid(CombatComponent->EquippedMeleeWeapon))
	{
		return CombatComponent->EquippedMeleeWeapon;
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Bow) && IsValid(CombatComponent->EquippedRangedWeapon))
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
	if (ASLBaseWeapon* MeleeWeapon = CombatComponent->EquippedMeleeWeapon)
	{
		MeleeWeapon->GetWeapon()->SetSimulatePhysics(true);
		MeleeWeapon->GetWeapon()->SetEnableGravity(true);
		MeleeWeapon->GetWeapon()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	if (ASLBaseWeapon* RangedWeapon = CombatComponent->EquippedRangedWeapon)
	{
		RangedWeapon->GetWeapon()->SetSimulatePhysics(true);
		RangedWeapon->GetWeapon()->SetEnableGravity(true);
		RangedWeapon->GetWeapon()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
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

ASLCharacterBase* ASLCharacterBase::GetLockOnTarget_Implementation()
{
	return LockTarget;
}

void ASLCharacterBase::SetLockOnTarget_Implementation(ASLCharacterBase* InLockTarget)
{
	LockTarget = InLockTarget;
}

bool ASLCharacterBase::IsStaring_Implementation()
{
	return bIsStaring;
}

void ASLCharacterBase::SetbIsStaring_Implementation(const bool InBool)
{
	bIsStaring = InBool;
}

int32 ASLCharacterBase::GetComboCode_Implementation()
{
	return CombatComponent->ComboCode;
}

void ASLCharacterBase::SetComboCode_Implementation(int32 InCode)
{
	CombatComponent->ComboCode = InCode;
}

void ASLCharacterBase::InitAbilityActorInfo()
{
}

void ASLCharacterBase::FocusOnTarget()
{
	if (IsValid(LockTarget))
	{
		const FVector LockActorRotation = FVector(LockTarget->GetActorLocation().X, LockTarget->GetActorLocation().Y,
		                                          LockTarget->GetActorLocation().Z - 150.f);
		const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
			GetActorLocation(), LockActorRotation);
		FRotator SmoothControllerRotation = UKismetMathLibrary::RInterpTo(GetController()->GetControlRotation(),
		                                                                  TargetRotation,
		                                                                  GetWorld()->DeltaTimeSeconds, 0.f);
		FRotator SmoothActorRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(),
		                                                             TargetRotation,
		                                                             GetWorld()->DeltaTimeSeconds, 0.f);
		SmoothControllerRotation = FRotator(GetController()->GetControlRotation().Pitch, SmoothControllerRotation.Yaw,
		                                    GetController()->GetControlRotation().Roll);
		SmoothActorRotation = FRotator(GetActorRotation().Pitch, SmoothActorRotation.Yaw, GetActorRotation().Roll);
		if (!AbilitySystemComponent->HasMatchingGameplayTag(FSLGameplayTags::Get().Abilities_Roll))
		{
			SetActorRotation(SmoothActorRotation);
		}
		GetController()->SetControlRotation(SmoothControllerRotation);
		bIsStaring = true;
	}
}

void ASLCharacterBase::UnFocusTarget()
{
	bIsStaring = false;
	if (!IsValid(LockTarget))
	{
	}
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
	if (OverlappingWeapon && OverlappingWeapon->GetWeaponState() != EWeaponState::EWS_Equipped)
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
		if (OverlappingWeapon && OverlappingWeapon->GetWeaponState() != EWeaponState::EWS_Equipped)
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
