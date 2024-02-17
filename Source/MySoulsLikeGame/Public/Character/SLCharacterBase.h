// copyright asun

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SLCharacterBase.generated.h"

UCLASS(Abstract)
class MYSOULSLIKEGAME_API ASLCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:	
	ASLCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category="Combat");
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
