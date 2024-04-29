// copyright aaaasun


#include "Actor/SLProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MySoulsLikeGame/MySoulsLikeGame.h"

ASLProjectile::ASLProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(Box);
	Box->SetCollisionObjectType(ECC_Projectile);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Box->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->ProjectileGravityScale = 0.98f;
	ProjectileMovement->bRotationFollowsVelocity = true;
}

void ASLProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSpan);
	Box->OnComponentHit.AddDynamic(this, &ASLProjectile::OnHit);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void ASLProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();
	}
	Super::Destroyed();
}

void ASLProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          FVector NormalImpulse, const FHitResult& Hit)
{
	if (DamageEffectSpecHandle.Data.IsValid() &&
		DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return;
	}
	if (!bHit)
	{
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();
	}
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			FGameplayEffectContextHandle EffectContextHandle = DamageEffectSpecHandle.Data.Get()->GetContext();
			FGameplayEffectContext* EffectContext = EffectContextHandle.Get();
			EffectContext->AddHitResult(Hit);

			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		Destroyed();
	}
	else
	{
		bHit = true;
	}
}

/*
void ASLProjectile::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
}
*/
