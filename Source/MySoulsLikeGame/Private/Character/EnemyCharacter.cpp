// copyright asun


#include "Character/EnemyCharacter.h"

#include "AbilitySystem/SLAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/SLAbilitySystemComponent.h"
#include "AbilitySystem/SLAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "MySoulsLikeGame/MySoulsLikeGame.h"
#include "UI/Widgets/SLUserWidget.h"

AEnemyCharacter::AEnemyCharacter()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);

	AbilitySystemComponent = CreateDefaultSubobject<USLAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<USLAttributeSet>("AttributeSet");

	AttributeBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	AttributeBar->SetupAttachment(GetRootComponent());
}

int32 AEnemyCharacter::GetPlayerLevel()
{
	return Level;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

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

		OnHealthChanged.Broadcast(SLAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(SLAttributeSet->GetMaxHealth());
		OnStaminaChanged.Broadcast(SLAttributeSet->GetStamina());
		OnMaxStaminaChanged.Broadcast(SLAttributeSet->GetMaxStamina());
	}
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<USLAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	USLAbilitySystemBlueprintLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
