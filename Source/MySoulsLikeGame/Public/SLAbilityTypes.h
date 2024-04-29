#pragma once
#include "GameplayEffectTypes.h"
#include "SLAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FSLGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	bool IsCriticalHit() const { return bIsCriticalHit; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

	/** 返回用于序列化的实际结构体，子类必须覆盖此值！ */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	/** 生成该上下文的副本，用于复制以便日后修改 */
	virtual FGameplayEffectContext* Duplicate() const
	{
		FGameplayEffectContext* NewContext = new FGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** 自定义序列化，子类必须重载该功能
	 *  NetSerialize：序列化和反序列化
	 *	FArchive:（可以储存序列化数据）归档文件的基类，可以字节顺序中立的方式用于加载、保存和垃圾回收。
	 *	UPackageMap:将对象和名称映射到索引或从索引映射到对象和名称，以便进行网络通信。
	 */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:
	UPROPERTY()
	bool bIsCriticalHit = false;
};

template <>
struct TStructOpsTypeTraits<FSLGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FSLGameplayEffectContext>
{
	enum
	{
		//WithNetSerializer:struct 有一个 NetSerialize 函数，用于将其状态序列化到用于网络复制的 FArchive 中。
		WithNetSerializer = true,
		//WithCopy:struct 可以通过其 copy 赋值操作符进行复制。
		WithCopy = true // 有必要将 TSharedPtr<FHitResult> 数据复制到其他地方
	};
};
