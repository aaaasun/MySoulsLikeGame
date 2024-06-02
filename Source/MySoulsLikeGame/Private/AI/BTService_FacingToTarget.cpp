// copyright aaaasun


#include "AI/BTService_FacingToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"

void UBTService_FacingToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = AIOwner->GetPawn();
	if (const AActor* TargetToFollow = Cast<AActor>(
		UBTFunctionLibrary::GetBlackboardValueAsObject(this, TargetToFollowSelector)))
	{
		// ICombatInterface::Execute_UpdateFacingTarget(OwningPawn, TargetToFollow->GetActorLocation());
		const FVector LockActorRotation = FVector(TargetToFollow->GetActorLocation().X,
		                                          TargetToFollow->GetActorLocation().Y,
		                                          TargetToFollow->GetActorLocation().Z - 150.f);
		const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
			OwningPawn->GetActorLocation(), LockActorRotation);
		FRotator SmoothActorRotation = UKismetMathLibrary::RInterpTo(OwningPawn->GetActorRotation(),
		                                                             TargetRotation,
		                                                             GetWorld()->DeltaTimeSeconds, 0.f);
		SmoothActorRotation = FRotator(OwningPawn->GetActorRotation().Pitch, SmoothActorRotation.Yaw,
		                               OwningPawn->GetActorRotation().Roll);
		OwningPawn->SetActorRotation(SmoothActorRotation);
	}
}
