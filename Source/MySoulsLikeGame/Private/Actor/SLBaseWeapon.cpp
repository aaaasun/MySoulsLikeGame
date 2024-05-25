// copyright aaaasun


#include "Actor/SLBaseWeapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/SLAbilitySystemBlueprintLibrary.h"
#include "Character/PlayerCharacter.h"
#include "Character/SLCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASLBaseWeapon::ASLBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	SetRootComponent(Weapon);
	Weapon->SetCollisionResponseToAllChannels(ECR_Block);
	Weapon->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComponent);
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);

	PickupArea = CreateDefaultSubobject<USphereComponent>(TEXT("PickupArea"));
	PickupArea->SetupAttachment(RootComponent);
	PickupArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
}

void ASLBaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		PickupArea->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PickupArea->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

		PickupArea->OnComponentBeginOverlap.AddDynamic(this, &ASLBaseWeapon::OnPickupAreaOverlap);
		PickupArea->OnComponentEndOverlap.AddDynamic(this, &ASLBaseWeapon::OnPickupAreaEndOverlap);
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &ASLBaseWeapon::OnCapsuleOverlap);
	}
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
}

void ASLBaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASLBaseWeapon, WeaponState);
	DOREPLIFETIME(ASLBaseWeapon, WeaponClass);
	DOREPLIFETIME(ASLBaseWeapon, AttachSocketName);
}

void ASLBaseWeapon::ShowPickupWidget(bool bShowWidget)
{
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(bShowWidget);
	}
}

USkeletalMeshComponent* ASLBaseWeapon::GetWeapon()
{
	return Weapon;
}

void ASLBaseWeapon::Destroyed()
{
	SetLifeSpan(LifeSpan);
	Super::Destroyed();
}

void ASLBaseWeapon::OnPickupAreaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter && PlayerCharacter->ActorHasTag("Player"))
	{
		PlayerCharacter->SetOverlappingWeapon(this);
	}
}

void ASLBaseWeapon::OnPickupAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter && PlayerCharacter->ActorHasTag("Player"))
	{
		PlayerCharacter->SetOverlappingWeapon(nullptr);
	}
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

EWeaponState ASLBaseWeapon::GetWeaponState()
{
	return WeaponState;
}

void ASLBaseWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		PickupArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	default: ;
	}
}

EWeaponClass ASLBaseWeapon::GetWeaponClass() const
{
	return WeaponClass;
}

FName ASLBaseWeapon::GetAttachSocketName() const
{
	return AttachSocketName;
}

void ASLBaseWeapon::BeginWeaponAttack()
{
	bIsAttacking = true;
	if (HasAuthority())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
		Capsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void ASLBaseWeapon::EndWeaponAttack()
{
	bIsAttacking = false;
	if (HasAuthority())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
		Capsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}

void ASLBaseWeapon::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp,
                                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsAttacking) return;
	if (!DamageEffectSpecHandle.Data.IsValid() ||
		DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return;
	}
	if (!USLAbilitySystemBlueprintLibrary::IsNotFriend(
		DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser(),
		OtherActor))
	{
		return;
	}
	if (!bHit)
	{
	}
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
	}
	else
	{
		bHit = true;
	}
}

void ASLBaseWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		PickupArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	default: ;
	}
}
