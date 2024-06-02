// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SLAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API ASLAIController : public AAIController
{
	GENERATED_BODY()

public:
	ASLAIController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> AIPerception;
};
