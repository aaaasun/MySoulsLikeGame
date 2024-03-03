// copyright asun


#include "UI/Widgets/SLUserWidget.h"

void USLUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
