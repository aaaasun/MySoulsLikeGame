// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FacingToTarget.generated.h"

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API UBTService_FacingToTarget : public UBTService_BlueprintBase
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector TargetToFollowSelector;
};
