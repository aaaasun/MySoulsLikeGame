// copyright asun

#pragma once

#include "CoreMinimal.h"
#include "Character/SLCharacterBase.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API AEnemyCharacter : public ASLCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
};
