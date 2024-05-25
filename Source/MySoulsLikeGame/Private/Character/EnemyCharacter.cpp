// copyright asun


#include "Character/EnemyCharacter.h"

#include "SLGameplayTags.h"
#include "AbilitySystem/SLAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/SLAbilitySystemComponent.h"
#include "AbilitySystem/SLAttributeSet.h"
#include "Actor/SLBaseWeapon.h"
#include "AI/SLAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widgets/SLUserWidget.h"

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<USLAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<USLAttributeSet>("AttributeSet");

	AttributeBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	AttributeBar->SetupAttachment(GetRootComponent());
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//AI只由服务器控制
	if (!HasAuthority()) return;

	SLAIController = Cast<ASLAIController>(NewController);
	SLAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	SLAIController->RunBehaviorTree(BehaviorTree);
	SLAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	SLAIController->GetBlackboardComponent()->SetValueAsBool(
		FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);

	SpawnDefaultWeapon();
}

int32 AEnemyCharacter::GetPlayerLevel()
{
	return Level;
}

void AEnemyCharacter::Die()
{
	SetLifeSpan(LifeSpan);
	if (SLAIController) SLAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	Super::Die();
}

void AEnemyCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AEnemyCharacter::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		USLAbilitySystemBlueprintLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);

		SpawnDefaultWeapon();
		AbilitySystemComponent->TryActivateAbilityByClass(WeaponInitializationAbility);
	}

	//显示敌人的面板,同OverlayWidgetController,也可以封装到一个类里
	if (USLUserWidget* SLUserWidget = Cast<USLUserWidget>(AttributeBar->GetUserWidgetObject()))
	{
		SLUserWidget->SetWidgetController(this);
	}

	if (const USLAttributeSet* SLAttributeSet = Cast<USLAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetMaxHealthAttribute()).
		                        AddLambda(
			                        [this](const FOnAttributeChangeData& Data)
			                        {
				                        OnMaxHealthChanged.Broadcast(Data.NewValue);
			                        });

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetStaminaAttribute()).
		                        AddLambda(
			                        [this](const FOnAttributeChangeData& Data)
			                        {
				                        OnStaminaChanged.Broadcast(Data.NewValue);
			                        }
		                        );

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(SLAttributeSet->GetMaxStaminaAttribute()).
		                        AddLambda(
			                        [this](const FOnAttributeChangeData& Data)
			                        {
				                        OnMaxStaminaChanged.Broadcast(Data.NewValue);
			                        }
		                        );

		AbilitySystemComponent->RegisterGameplayTagEvent(FSLGameplayTags::Get().Effect_HitReact,
		                                                 EGameplayTagEventType::NewOrRemoved)
		                      .AddUObject(
			                      this, &AEnemyCharacter::HitReactTagChanged
		                      );

		OnHealthChanged.Broadcast(SLAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(SLAttributeSet->GetMaxHealth());
		OnStaminaChanged.Broadcast(SLAttributeSet->GetStamina());
		OnMaxStaminaChanged.Broadcast(SLAttributeSet->GetMaxStamina());
	}
}

void AEnemyCharacter::SpawnDefaultWeapon()
{
	if (DefaultMeleeWeaponClass && Execute_GetCombatWeapon(this, FSLGameplayTags::Get().Montage_Attack_Weapon) ==
		nullptr)
	{
		ASLBaseWeapon* DefaultMeleeWeapon = GetWorld()->SpawnActor<ASLBaseWeapon>(DefaultMeleeWeaponClass);
		CombatComponent->SetMeleeWeapon(DefaultMeleeWeapon);
	}
	if (DefaultRangedWeaponClass && Execute_GetCombatWeapon(this, FSLGameplayTags::Get().Montage_Attack_Bow) == nullptr)
	{
		ASLBaseWeapon* DefaultRangedWeapon = GetWorld()->SpawnActor<ASLBaseWeapon>(DefaultRangedWeaponClass);
		CombatComponent->SetRangedWeapon(DefaultRangedWeapon);
	}
}

void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (SLAIController && SLAIController->GetBlackboardComponent())
	{
		SLAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<USLAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	USLAbilitySystemBlueprintLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
