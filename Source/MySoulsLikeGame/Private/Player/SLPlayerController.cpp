// copyright asun


#include "Player/SLPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ASLPlayerController::ASLPlayerController()
{
	bReplicates = true;
}

void ASLPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(SLContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(SLContext, 0);
}

void ASLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASLPlayerController::Move);
}

void ASLPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>(); //向量
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); //偏移量
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); //向前方向
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //向右方向

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
