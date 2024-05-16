// copyright aaaasun


#include "AbilitySystem/SLAbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "SLAbilityTypes.h"
#include "Game/SLGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SLPlayerState.h"
#include "UI/HUD/SLHUD.h"
#include "UI/WidgetController/SLWidgetController.h"

UOverlayWidgetController* USLAbilitySystemBlueprintLibrary::GetOverlayWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ASLHUD* SLHUD = Cast<ASLHUD>(PC->GetHUD()))
		{
			ASLPlayerState* PS = PC->GetPlayerState<ASLPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return SLHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* USLAbilitySystemBlueprintLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ASLHUD* SLHUD = Cast<ASLHUD>(PC->GetHUD()))
		{
			ASLPlayerState* PS = PC->GetPlayerState<ASLPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return SLHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void USLAbilitySystemBlueprintLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                                   ECharacterClass CharacterClass, int32 Level,
                                                                   UAbilitySystemComponent* ASC)
{
	const ASLGameModeBase* SLGameMode = Cast<ASLGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (SLGameMode == nullptr) return;

	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = SLGameMode->CharacterClassInfo;
	const FCharacterClassDefaultInfo CharacterClassDefaultInfo = CharacterClassInfo->
		GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void USLAbilitySystemBlueprintLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
                                                            UAbilitySystemComponent* ASC,
                                                            ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* USLAbilitySystemBlueprintLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ASLGameModeBase* SLGameMode = Cast<ASLGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (SLGameMode == nullptr) return nullptr;
	return SLGameMode->CharacterClassInfo;
}

bool USLAbilitySystemBlueprintLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FSLGameplayEffectContext* SLEffectContext = static_cast<const FSLGameplayEffectContext*>(
		EffectContextHandle.Get()))
	{
		return SLEffectContext->IsCriticalHit();
	}
	return false;
}

void USLAbilitySystemBlueprintLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
                                                        bool bInIsCriticalHit)
{
	if (FSLGameplayEffectContext* SLEffectContext = static_cast<FSLGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		SLEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void USLAbilitySystemBlueprintLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                                  TArray<AActor*>& OutOnHitActors,
                                                                  const TArray<AActor*>& ActorsToIgnore,
                                                                  const FVector& Extent,
                                                                  const FVector& Start,
                                                                  const FVector& End)
{
	//参考内置的UGameplayStatics::ApplyRadialDamageWithFalloff
	FCollisionQueryParams CapsuleParams;
	CapsuleParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,
	                                                             EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FHitResult> OutHits;
		World->SweepMultiByObjectType(OutHits, Start, End, FQuat::Identity,
		                              FCollisionObjectQueryParams(
			                              FCollisionObjectQueryParams::InitType::AllDynamicObjects),
		                              FCollisionShape::MakeCapsule(Extent), CapsuleParams);
		for (FHitResult& OutHit : OutHits)
		{
			if (OutHit.GetActor()->Implements<UCombatInterface>() &&
				!ICombatInterface::Execute_IsDead(OutHit.GetActor()))
			{
				OutOnHitActors.AddUnique(ICombatInterface::Execute_GetAvatar(OutHit.GetActor()));
			}
		}
	}
}

bool USLAbilitySystemBlueprintLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}
