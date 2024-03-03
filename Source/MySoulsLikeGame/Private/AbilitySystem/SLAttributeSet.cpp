// copyright asun


#include "AbilitySystem/SLAttributeSet.h"
#include "Net/UnrealNetwork.h"

USLAttributeSet::USLAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitStamina(100.f);
	InitMaxStamina(100.f);
}

//类必须有这个函数，以注册用于复制的变量
void USLAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//复制属性
	DOREPLIFETIME_CONDITION_NOTIFY(USLAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USLAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USLAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(USLAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	//COND_None:无条件复制；REPNOTIFY_Always：总是通知（Changes是更改值时通知）
}

void USLAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	//通知AS Health被复制,AS可以记录这变化并跟踪旧Value，以防需要回滚
	GAMEPLAYATTRIBUTE_REPNOTIFY(USLAttributeSet, Health, OldHealth);
}

void USLAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USLAttributeSet, MaxHealth, OldMaxHealth);
}

void USLAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USLAttributeSet, MaxHealth, OldStamina);
}

void USLAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USLAttributeSet, MaxHealth, OldMaxStamina);
}
