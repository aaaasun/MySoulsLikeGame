// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Components/WidgetComponent.h"
#include "Data/SLWeaponItem.h"
#include "GameFramework/Actor.h"
#include "SLBaseWeapon.generated.h"

class USphereComponent;
class UCapsuleComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName="Initial State"),
	EWS_Equipped UMETA(DisplayName="Equipped State"),
	EWS_Dropped UMETA(DisplayName="Dropped State"),

	EWS_MAX UMETA(DisplayName="DefaultMAX")
};

UENUM(BlueprintType)
enum class EWeaponClass : uint8
{
	Melee,
	Ranged
};


UCLASS()
class MYSOULSLIKEGAME_API ASLBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASLBaseWeapon();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ShowPickupWidget(bool bShowWidget);

	UFUNCTION(BlueprintCallable, Category="Weapon Properties")
	USkeletalMeshComponent* GetWeapon();

	UPROPERTY(BlueprintReadWrite)
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachToCharacter(const FName WeaponSocket);

	UFUNCTION(BlueprintCallable, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GetWeaponAbilities();

	UFUNCTION()
	float GetLifeSpanAfterDeath() const;

	UFUNCTION()
	void SetWeaponState(EWeaponState State);

	UFUNCTION(BlueprintCallable, Category="Weapon Properties")
	EWeaponClass GetWeaponClass() const;

	UFUNCTION(BlueprintCallable, Category="Weapon Properties")
	FName GetAttachSocketName() const;

	FORCEINLINE USphereComponent* GetPickArea() const { return PickupArea; }

	UFUNCTION(BlueprintCallable, Category="Combat")
	void BeginWeaponAttack();

	UFUNCTION(BlueprintCallable, Category="Combat")
	void EndWeaponAttack();

protected:
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bIsAttacking = false;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> WeaponAbilities;

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnPickupAreaOverlap(UPrimitiveComponent* OverlappedComponent,
	                                 AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp,
	                                 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnPickupAreaEndOverlap(UPrimitiveComponent* OverlappedComponent,
	                                    AActor* OtherActor,
	                                    UPrimitiveComponent* OtherComp,
	                                    int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp,
	                              int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	TObjectPtr<USphereComponent> PickupArea;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category="Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Weapon Properties")
	EWeaponClass WeaponClass;

	UPROPERTY(Replicated, EditDefaultsOnly, Category="Weapon Properties")
	FName AttachSocketName;

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(VisibleAnywhere, Category="Weapon Properties")
	TObjectPtr<UWidgetComponent> PickupWidget;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 5.f;

	bool bHit = false;
};
