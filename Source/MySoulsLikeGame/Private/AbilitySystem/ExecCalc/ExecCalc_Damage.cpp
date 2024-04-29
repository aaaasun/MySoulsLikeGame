// copyright aaaasun


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "SLAbilityTypes.h"
#include "SLGameplayTags.h"
#include "AbilitySystem/SLAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/SLAttributeSet.h"

struct SLDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;

	SLDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(USLAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USLAttributeSet, PhysicalResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(USLAttributeSet, FireResistance, Target, false);

		TagsToCaptureDef.Add(FSLGameplayTags::Get().Attributes_Secondary_Defense, DefenseDef);
		TagsToCaptureDef.Add(FSLGameplayTags::Get().Attributes_Resistance_Physical, PhysicalResistanceDef);
		TagsToCaptureDef.Add(FSLGameplayTags::Get().Attributes_Resistance_Fire, FireResistanceDef);
	}
};

static const SLDamageStatics& DamageStatics()
{
	static SLDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	//获取按caller设置的伤害大小
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FSLGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTypeTag = Pair.Value;

		checkf(SLDamageStatics().TagsToCaptureDef.Contains(ResistanceTypeTag),
		       TEXT("ExecCalc中的TagsToCaptureDef里没有Tag:[%s]"), *ResistanceTypeTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = SLDamageStatics().TagsToCaptureDef[
			ResistanceTypeTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);

		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;

		Damage += DamageTypeValue;
	}

	//捕获目标防御力
	float TargetDefense = 0.;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvaluateParameters,
	                                                           TargetDefense);
	TargetDefense = FMath::Max<float>(TargetDefense, 0.f);

	//根据防御力抵挡伤害
	Damage *= (100 - TargetDefense) / 100.f;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	//击中弱点暴击
	const FHitResult* Hit = EffectContextHandle.GetHitResult();
	bool IsCriticalHit = false;
	if (Hit && Hit->BoneName == "head")
	{
		IsCriticalHit = true;
		Damage *= 2.f;
	}
	USLAbilitySystemBlueprintLibrary::SetIsCriticalHit(EffectContextHandle, IsCriticalHit);

	const FGameplayModifierEvaluatedData EvaluatedData(USLAttributeSet::GetIncomingDamageAttribute(),
	                                                   EGameplayModOp::Additive,
	                                                   Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
