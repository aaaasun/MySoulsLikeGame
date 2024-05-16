// copyright aaaasun


#include "Actor/SLBaseWeapon.h"

#include "Components/CapsuleComponent.h"

// Sets default values
ASLBaseWeapon::ASLBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// void ASLBaseWeapon::AttachActor(FName SocketName)
// {
// 	const ASLCharacterBase* Character = Cast<ASLCharacterBase>(GetOwner());
// 	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,false);
// 	AttachToComponent(Character->GetMesh(), AttachmentRules, SocketName);
// }

void ASLBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

