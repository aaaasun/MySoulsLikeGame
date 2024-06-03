// copyright asun

#pragma once

#include "CoreMinimal.h"
#include "Character/SLCharacterBase.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API APlayerCharacter : public ASLCharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float BaseWalkSpeed = 600.f;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	/**Combat Interaction**/
	virtual int32 GetPlayerLevel() override;
	virtual AActor* GetClosestEnemy_Implementation(float Radius, float Length) override;
	/**end Combat Interaction**/

protected:
	UPROPERTY()
	bool bAbilitiesInitialized = false;

	void AddStartupAbilities();

private:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	virtual void InitAbilityActorInfo() override;
};
