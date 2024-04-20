// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "HitResultTargetData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitResultTargetDataSignature, const FGameplayAbilityTargetDataHandle&,
                                            DataHandle);

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API UHitResultTargetData : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta=(DisplayName="HitResultTargetData", HidePin="OwningAbility", DefaultToSelf="OwningAbility",
			BlueprintInternalUseOnly="true"))
	static UHitResultTargetData* CreateHitResultTargetData(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FHitResultTargetDataSignature ValidData;

private:
	virtual void Activate() override;

	void SendUnderCrosshairsData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);

	void TraceUnderCrosshairs(float TraceDistance, FHitResult& TraceHitResult);
};
