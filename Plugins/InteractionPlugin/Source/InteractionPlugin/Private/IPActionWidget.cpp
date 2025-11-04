// Copyright pjdevs. All Rights Reserved.


#include "IPActionWidget.h"

void UIPActionWidget::NativePreConstruct()
{
	Super::NativeConstruct();

	if (InputAction)
	{
		SetActionVisual(InputAction);
	}
}

void UIPActionWidget::SetAction(const UInputAction* InInputAction)
{
	InputAction = InInputAction;
}
