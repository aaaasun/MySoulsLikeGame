// copyright aaaasun


#include "Character/SLCharacterBase.h"

ASLCharacterBase::ASLCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;


	Weapon=CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASLCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

