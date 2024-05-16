// copyright asun


#include "Character/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/SLAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/SLPlayerController.h"
#include "Player/SLPlayerState.h"
#include "UI/HUD/SLHUD.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}


void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//为服务端初始化Ability Actor Info
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//为客户端初始化Ability Actor Info
	InitAbilityActorInfo();
}

int32 APlayerCharacter::GetPlayerLevel()
{
	const ASLPlayerState* SLPlayerState = GetPlayerState<ASLPlayerState>();
	check(SLPlayerState);
	return SLPlayerState->GetPlayerLevel();
}

void APlayerCharacter::InitAbilityActorInfo()
{
	ASLPlayerState* SLPlayerState = GetPlayerState<ASLPlayerState>();
	check(SLPlayerState);
	AbilitySystemComponent = SLPlayerState->GetAbilitySystemComponent();
	AttributeSet = SLPlayerState->GetAttributeSet();
	AbilitySystemComponent->InitAbilityActorInfo(SLPlayerState, this);
	Cast<USLAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	//初始化Overlay
	if (ASLPlayerController* SLPlayerController = Cast<ASLPlayerController>(GetController()))
	{
		if (ASLHUD* SLHUD = Cast<ASLHUD>(SLPlayerController->GetHUD()))
		{
			SLHUD->InitOverlay(SLPlayerController, SLPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}
