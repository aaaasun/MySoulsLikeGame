// copyright asun

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacterBase.generated.h"

UCLASS(Abstract)
class MYSOULSLIKEGAME_API AMyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:	
	AMyCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category="Combat");
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
