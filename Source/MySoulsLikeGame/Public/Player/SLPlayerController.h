// copyright asun

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SLPlayerController.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API ASLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASLPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> SLContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;
	
	void Move(const FInputActionValue& InputActionValue);

	void Look(const FInputActionValue& InputActionValue);
};
