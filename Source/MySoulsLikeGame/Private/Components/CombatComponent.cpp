// copyright aaaasun


#include "Components/CombatComponent.h"

#include "Actor/SLBaseWeapon.h"
#include "Character/SLCharacterBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCombatComponent, EquippedMeleeWeapon);
	DOREPLIFETIME(UCombatComponent, EquippedRangedWeapon);
	DOREPLIFETIME(UCombatComponent, ComboCode);
}

void UCombatComponent::EquipWeapon(ASLBaseWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr) return;
	if (WeaponToEquip->GetWeaponClass() == EWeaponClass::Melee)
	{
		SetMeleeWeapon(WeaponToEquip);
		EquippedMeleeWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		const USkeletalMeshSocket* AttachSocket = Character->GetMesh()->GetSocketByName(
			WeaponToEquip->GetAttachSocketName());
		if (AttachSocket)
		{
			AttachSocket->AttachActor(EquippedMeleeWeapon, Character->GetMesh());
		}
		EquippedMeleeWeapon->SetOwner(Character);
	}
	if (WeaponToEquip->GetWeaponClass() == EWeaponClass::Ranged)
	{
		SetRangedWeapon(WeaponToEquip);
		EquippedRangedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		const USkeletalMeshSocket* AttachSocket = Character->GetMesh()->GetSocketByName(
			WeaponToEquip->GetAttachSocketName());
		if (AttachSocket)
		{
			AttachSocket->AttachActor(EquippedRangedWeapon, Character->GetMesh());
		}
		EquippedRangedWeapon->SetOwner(Character);
	}
}

void UCombatComponent::AddWeaponAbilities(ASLBaseWeapon* EquippedWeapon)
{
	if (Character == nullptr || EquippedWeapon == nullptr) return;
	Character->AddCharacterAbilities(EquippedWeapon->GetWeaponAbilities());
}

void UCombatComponent::SetMeleeWeapon(ASLBaseWeapon* InWeapon)
{
	EquippedMeleeWeapon = InWeapon;
}

void UCombatComponent::SetRangedWeapon(ASLBaseWeapon* InWeapon)
{
	EquippedRangedWeapon = InWeapon;
}
