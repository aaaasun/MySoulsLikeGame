// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "SLProjectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class MYSOULSLIKEGAME_API ASLProjectile : public AActor
{
	GENERATED_BODY()

public:
	ASLProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                  FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

	bool bHit = false;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;
};
