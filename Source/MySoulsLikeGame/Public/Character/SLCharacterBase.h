// copyright asun

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Abilities/SLDamageGameplayAbility.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "UI/Widgets/SLUserWidget.h"
#include "SLCharacterBase.generated.h"

class UCombatComponent;
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
	virtual UCombatComponent* GetCombatComponent_Implementation() override;
	virtual ASLBaseWeapon* GetOverlapWeapon_Implementation() override;
	virtual ASLBaseWeapon* GetCombatWeapon_Implementation(const FGameplayTag& MontageTag) override;
	virtual USkeletalMeshComponent* GetCharacterMesh_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual ASLCharacterBase* GetLockOnTarget_Implementation() override;
	virtual void SetLockOnTarget_Implementation(ASLCharacterBase* InLockTarget) override;
	virtual bool IsStaring_Implementation() override;
	virtual void SetbIsStaring_Implementation(const bool InBool) override;
	virtual int32 GetComboCode_Implementation() override;
	virtual void SetComboCode_Implementation(int32 InCode) override;
	/** end Combat Interface */

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	bool EnableComboPeriod = true;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category="Abilities")
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InAbilities);

	UFUNCTION(BlueprintCallable, Category="Combat")
	void JumpSectionForCombo(FName NextSection);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCombatComponent> CombatComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASLBaseWeapon> DefaultMeleeWeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ASLBaseWeapon> DefaultRangedWeaponClass;

	UPROPERTY(Replicated)
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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Combat")
	TObjectPtr<ASLCharacterBase> LockTarget;

	UFUNCTION()
	void FocusOnTarget();

	UFUNCTION()
	void UnFocusTarget();

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	virtual void InitializeDefaultAttributes() const;

private:
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<ASLBaseWeapon> OverlappingWeapon;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(Replicated)
	bool bIsStaring = false;

	UFUNCTION()
	void OnRep_OverlappingWeapon(ASLBaseWeapon* LastWeapon);

public:
	void SetOverlappingWeapon(ASLBaseWeapon* InWeapon);
};
