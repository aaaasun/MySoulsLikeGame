// copyright asun


#include "Player/SLPlayerState.h"
#include "AbilitySystem/SLAbilitySystemComponent.h"
#include "AbilitySystem/SLAttributeSet.h"
#include "Net/UnrealNetwork.h"

ASLPlayerState::ASLPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<USLAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<USLAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

void ASLPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//最基本的宏用于将某些内容标记为已复制
	DOREPLIFETIME(ASLPlayerState, Level);
}

UAbilitySystemComponent* ASLPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASLPlayerState::OnRep_Level(int32 OldLevel)
{
}
