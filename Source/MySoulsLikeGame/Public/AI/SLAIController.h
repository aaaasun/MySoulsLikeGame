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
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
