// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


class ASLBaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYSOULSLIKEGAME_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	friend class ASLCharacterBase;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void EquipWeapon(ASLBaseWeapon* WeaponToEquip);

	UFUNCTION(BlueprintCallable, Category="Abilities")
	void AddWeaponAbilities(ASLBaseWeapon* EquippedWeapon);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetMeleeWeapon(ASLBaseWeapon* InWeapon);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetRangedWeapon(ASLBaseWeapon* InWeapon);

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<ASLCharacterBase> Character;

	UPROPERTY(Replicated)
	TObjectPtr<ASLBaseWeapon> EquippedMeleeWeapon;

	UPROPERTY(Replicated)
	TObjectPtr<ASLBaseWeapon> EquippedRangedWeapon;
};
