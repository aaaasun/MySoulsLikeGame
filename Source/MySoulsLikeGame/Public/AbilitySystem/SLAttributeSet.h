// copyright asun

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SLAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties()
	{
	}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

//TStaticFuncPtr对于任何静态函数指针签名都是通用的
template <class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API USLAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	USLAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	/**
	* Primary Attributes
	* 活力、精力、力量、耐力、恢复力、心力、运气
	*/
	//活力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Vigor, Category="Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Vigor);
	//精力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Energy, Category="Primary Attributes")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Energy);
	//力量
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, Category="Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Strength);
	//耐力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Endurance, Category="Primary Attributes")
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Endurance);
	//运气
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Luck, Category="Primary Attributes")
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Luck);

	/**
	 * Secondary Attributes
	 * 最大血量、最大体力、最大心力、攻击力、防御力、体力恢复
	 */
	//最大血量(活力影响
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, MaxHealth);
	//最大体力(精力影响
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxStamina, Category="Secondary Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, MaxStamina);
	//最大心力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMentalStrength, Category="Secondary Attributes")
	FGameplayAttributeData MaxMentalStrength;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, MaxMentalStrength);
	//攻击力(力量影响
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_AttackPower, Category="Secondary Attributes")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, AttackPower);
	//防御力(耐力影响
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Defense, Category="Secondary Attributes")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Defense);
	//体力恢复(耐力影响
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_StaminaRegeneration, Category="Secondary Attributes")
	FGameplayAttributeData StaminaRegeneration;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, StaminaRegeneration);

	/**
	 * Vital Attributes
	 * 血量、体力、心力
	 */
	//血量
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health;
	//这定义了一组用于访问和初始化属性的辅助函数:GetHealthAttribute、Get、Set、InitHealth，以避免手动编写这些函数。
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Health);
	//体力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Stamina, Category="Vital Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, Stamina);
	//心力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MentalStrength, Category="Vital Attributes")
	FGameplayAttributeData MentalStrength;
	ATTRIBUTE_ACCESSORS(USLAttributeSet, MentalStrength);


	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Energy(const FGameplayAttributeData& OldEnergy) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Endurance(const FGameplayAttributeData& OldEndurance) const;

	UFUNCTION()
	void OnRep_Luck(const FGameplayAttributeData& OldLuck) const;

	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldAttackPower) const;

	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldDefense) const;

	UFUNCTION()
	void OnRep_StaminaRegeneration(const FGameplayAttributeData& OldStaminaRegeneration) const;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;

	UFUNCTION()
	void OnRep_MentalStrength(const FGameplayAttributeData& OldMentalStrength) const;

	UFUNCTION()
	void OnRep_MaxMentalStrength(const FGameplayAttributeData& OldMaxMentalStrength) const;

private:
	static void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
};
