// copyright aaaasun


#include "Actor/SLBaseWeapon.h"

#include "Character/SLCharacterBase.h"
#include "GameFramework/Character.h"

// Sets default values
ASLBaseWeapon::ASLBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASLBaseWeapon::AttachToCharacter(const FName WeaponSocket)
{
	if (HasAuthority())
	{
		if (ASLCharacterBase* TargetCharacter = Cast<ASLCharacterBase>(GetOwner()))
		{
			//SnapToTarget会继承附加到其上的组件的转换，第二个参数表示是否开启物理模拟
			const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			AttachToComponent(TargetCharacter->GetMesh(), AttachmentRules, WeaponSocket);
		}
	}
}

TArray<TSubclassOf<UGameplayAbility>> ASLBaseWeapon::GetWeaponAbilities()
{
	return WeaponAbilities;
}

float ASLBaseWeapon::GetLifeSpanAfterDeath() const
{
	return LifeSpan;
}

void ASLBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASLBaseWeapon::Destroyed()
{
	SetLifeSpan(LifeSpan);
	Super::Destroyed();
}
