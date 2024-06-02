// copyright aaaasun


#include "AI/SLAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

ASLAIController::ASLAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
	check(AIPerception);
}

void ASLAIController::BeginPlay()
{
	Super::BeginPlay();
}
