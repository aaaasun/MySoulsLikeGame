// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SLBaseWeapon.generated.h"

class UCapsuleComponent;

UCLASS()
class MYSOULSLIKEGAME_API ASLBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASLBaseWeapon();
	
	// UFUNCTION(BlueprintCallable)
	// void AttachActor(FName SocketName);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> Capsule;
};
