// copyright asun

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SLGameModeBase.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API ASLGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
