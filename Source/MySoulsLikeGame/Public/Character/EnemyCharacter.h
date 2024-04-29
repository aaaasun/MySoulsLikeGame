// copyright asun

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/SLCharacterBase.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "EnemyCharacter.generated.h"

struct FGameplayTag;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API AEnemyCharacter : public ASLCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	/**Combat Interaction**/
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	/**end Combat Interaction**/

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangedSignature OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attribute")
	FOnAttributeChangedSignature OnMaxStaminaChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float LifeSpan = 5.f;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> AttributeBar;
};
