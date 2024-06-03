// copyright asun


#include "Character/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "CollisionDebugDrawingPublic.h"
#include "SLGameplayTags.h"
#include "AbilitySystem/SLAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/SLAbilitySystemComponent.h"
#include "Actor/SLBaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/SLPlayerController.h"
#include "Player/SLPlayerState.h"
#include "UI/HUD/SLHUD.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//为服务端初始化Ability Actor Info
	InitAbilityActorInfo();
	if (HasAuthority() && !bAbilitiesInitialized)
	{
		AddStartupAbilities();
		bAbilitiesInitialized = true;
	}
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//为客户端初始化Ability Actor Info
	InitAbilityActorInfo();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsValid(LockTarget))
	{
		FocusOnTarget();
		bool Dead = Execute_IsDead(LockTarget);
		if (Dead)
		{
			Execute_SetLockOnTarget(this, nullptr);
		}
	}
	if (!IsValid(LockTarget))
	{
		UnFocusTarget();
	}
}

int32 APlayerCharacter::GetPlayerLevel()
{
	const ASLPlayerState* SLPlayerState = GetPlayerState<ASLPlayerState>();
	check(SLPlayerState);
	return SLPlayerState->GetPlayerLevel();
}

AActor* APlayerCharacter::GetClosestEnemy_Implementation(float Radius, float Length)
{
	AActor* ClosestEnemy = nullptr;
	TArray<FHitResult> HitResults;
	const FVector Start = FollowCamera->GetComponentLocation();
	const FVector End = Start + FollowCamera->GetForwardVector() * Length;
	const FQuat Rot;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	FCollisionShape Sphere;
	Sphere.MakeSphere(Radius);
	Sphere.SetSphere(Radius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (GetWorld()->SweepMultiByObjectType(HitResults, Start, End, Rot, ObjectQueryParams, Sphere, Params))
	{
		float ClosestDistance = 5000.f;
		for (FHitResult Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && USLAbilitySystemBlueprintLibrary::IsNotFriend(this, HitActor) && !Execute_IsDead(HitActor))
			{
				float Distance = HitActor->GetDistanceTo(this);
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestEnemy = HitActor;
				}
			}
		}
	}
	return ClosestEnemy;
}

void APlayerCharacter::AddStartupAbilities()
{
	check(AbilitySystemComponent);
	if (HasAuthority())
	{
		// 授予能力，但仅限于服务器
		AddCharacterAbilities(StartupAbilities);
	}
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
