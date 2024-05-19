// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Data/SLWeaponItem.h"
#include "GameFramework/Actor.h"
#include "SLBaseWeapon.generated.h"

UCLASS()
class MYSOULSLIKEGAME_API ASLBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASLBaseWeapon();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachToCharacter(const FName WeaponSocket);

	UFUNCTION(BlueprintCallable, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GetWeaponAbilities();
	
	UFUNCTION()
	float GetLifeSpanAfterDeath() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Class Defaults")
	EWeaponClass WeaponClass = EWeaponClass::Melee;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> WeaponAbilities;

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

private:
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 5.f;
};
