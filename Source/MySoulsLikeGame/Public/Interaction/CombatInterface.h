// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/CombatComponent.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class UAnimMontage;
class AEnemyCharacter;

/**
 * 
 */
class MYSOULSLIKEGAME_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UCombatComponent* GetCombatComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ASLBaseWeapon* GetOverlapWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ASLBaseWeapon* GetCombatWeapon(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetCharacterMesh();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetClosestEnemy(float Radius, float Length);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ASLCharacterBase* GetLockOnTarget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetLockOnTarget(ASLCharacterBase* InLockTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsStaring();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetbIsStaring(const bool InBool);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHit(const FHitResult& HitResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AbilitiesCombo();
};
