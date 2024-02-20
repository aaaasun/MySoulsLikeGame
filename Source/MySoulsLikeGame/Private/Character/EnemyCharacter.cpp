// copyright asun


#include "Character/EnemyCharacter.h"

#include "AbilitySystem/SLAbilitySystemComponent.h"
#include "AbilitySystem/SLAttributeSet.h"

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<USLAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet=CreateDefaultSubobject<USLAttributeSet>("AttributeSet");
}
