// copyright asun


#include "UI/WidgetController/SLWidgetController.h"

void USLWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState=WCParams.PlayerState;
	AbilitySystemComponent=WCParams.AbilitySystemComponent;
	AttributeSet=WCParams.AttributeSet;
}

void USLWidgetController::BroadcastInitialValues()
{
	
}

void USLWidgetController::BindCallbacksToDependencies()
{
}
