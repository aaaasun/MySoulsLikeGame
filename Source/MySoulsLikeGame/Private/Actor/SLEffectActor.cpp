// copyright aaaasun


#include "Actor/SLEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

ASLEffectActor::ASLEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void ASLEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASLEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//UE的全局库函数直接获取ASC
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	//FGameplayEffectContextHandle结构体包装 FGameplayEffectContext 或子类的句柄，以允许其多态并正确复制
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();

	//设置创建此效果的对象
	EffectContextHandle.AddSourceObject(this);

	//FGameplayEffectSpecHandle结构体允许蓝图生成一次 GameplayEffectSpec，然后通过句柄引用它，以多次应用它/多个目标
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
		GameplayEffectClass, ActorLevel, EffectContextHandle);

	//返回的是const FGameplayEffectSpec& GameplayEffect,而Data是个TSharedPtr
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(
		*EffectSpecHandle.Data.Get());

	//判断Effect是否是Infinite
	const bool IsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy ==
		EGameplayEffectDurationType::Infinite;

	//如果是Infinite效果且蓝图中设计为是要消除Effect的策略，用一个Map存储使用的EffectHandle和ASC，后续消除Effect
	if (IsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void ASLEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void ASLEffectActor::EndOnOverlap(AActor* TargetActor)
{
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		//清除Effect后，无法从当前循环的容器中删除元素，beggin会报错，所以创建一个数组存下要删除的Handle，后续再遍历删除
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			//检查Map中是否有ASC=这里用了Infinite效果的ASC
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
