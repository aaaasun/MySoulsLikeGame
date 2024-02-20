// copyright asun


#include "Player/SLPlayerState.h"
#include "AbilitySystem/SLAbilitySystemComponent.h"
#include "AbilitySystem/SLAttributeSet.h"

ASLPlayerState::ASLPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<USLAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<USLAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* ASLPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
