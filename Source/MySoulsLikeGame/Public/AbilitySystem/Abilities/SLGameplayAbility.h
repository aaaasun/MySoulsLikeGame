// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SLGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API USLGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	USLGameplayAbility();

	//启用输入标签，用于在开始赋予角色标签时做检查
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;
};
