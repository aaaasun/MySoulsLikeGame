// copyright asun

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Abilities/SLDamageGameplayAbility.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "SLCharacterBase.generated.h"

class ASLBaseWeapon;
class USpringArmComponent;
class UCameraComponent;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;
class UAnimMontage;

UCLASS(Abstract)
class MYSOULSLIKEGAME_API ASLCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ASLCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/** Combat Interface */
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual USkeletalMeshComponent* GetCombatComponent_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	/** end Combat Interface */

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	bool EnableComboPeriod = true;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	ASLBaseWeapon* GetMeleeWeapon();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	ASLBaseWeapon* GetRangedWeapon();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetMeleeWeapon(ASLBaseWeapon* NewMeleeWeapon);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void SetRangedWeapon(ASLBaseWeapon* NewRangedWeapon);

	UFUNCTION(BlueprintCallable, Category="Abilities")
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InAbilities);

	UFUNCTION(BlueprintCallable, Category="Combat")
	void JumpSectionForCombo(FName NextSection);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASLBaseWeapon> DefaultMeleeWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName MeleeWeaponAttachSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASLBaseWeapon> DefaultRangedWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName RangedWeaponAttachSocket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Bow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> ArrowPouch;

	bool bDead = false;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	virtual void InitializeDefaultAttributes() const;

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon")
	void SpawnDefaultWeapon();

private:
	UPROPERTY()
	TObjectPtr<ASLBaseWeapon> MeleeWeapon;

	UPROPERTY()
	TObjectPtr<ASLBaseWeapon> RangedWeapon;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
