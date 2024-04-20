// copyright asun


#include "Player/SLPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SLGameplayTags.h"
#include "AbilitySystem/SLAbilitySystemComponent.h"
#include "Input/SLInputComponent.h"

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
	if (Subsystem)
	{
		Subsystem->AddMappingContext(SLContext, 0);
	}
}

void ASLPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FSLGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void ASLPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FSLGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
}

void ASLPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FSLGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
}

USLAbilitySystemComponent* ASLPlayerController::GetASC()
{
	if (SLAbilitySystemComponent == nullptr)
	{
		SLAbilitySystemComponent = Cast<USLAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return SLAbilitySystemComponent;
}

void ASLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	USLInputComponent* SLInputComponent = CastChecked<USLInputComponent>(InputComponent);
	SLInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASLPlayerController::Move);
	SLInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASLPlayerController::Look);
	SLInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                     &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ASLPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>(); //2d向量
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f); //偏移量
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); //向前方向
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //向右方向
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASLPlayerController::Look(const FInputActionValue& InputActionValue)
{
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}
