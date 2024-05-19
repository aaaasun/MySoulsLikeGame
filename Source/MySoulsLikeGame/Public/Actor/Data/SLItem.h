// copyright aaaasun

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SLItem.generated.h"

class USLGameplayAbility;
/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class MYSOULSLIKEGAME_API USLItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	USLItem()
		: Price(0)
		, MaxCount(1)
		, MaxLevel(1)
		, AbilityLevel(1)
	{}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FPrimaryAssetType ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Price;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Max")
	int32 MaxCount;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Max")
	bool IsConsumable() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Max")
	int32 MaxLevel;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<USLGameplayAbility> GrantedAbility;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	int32 AbilityLevel;
	
	UFUNCTION(BlueprintCallable, Category = "Item")
	FString GetIdentifierString() const;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
