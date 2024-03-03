// copyright asun

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SLUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYSOULSLIKEGAME_API USLUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)//在蓝图调用
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	//获取来自WidgetController的数据
	UFUNCTION(BlueprintImplementableEvent)//在C++可以声明函数（不能定义，蓝图重写）
	void WidgetControllerSet();
};
