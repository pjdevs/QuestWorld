// Copyright pjdevs. All Rights Reserved.


#include "IPActionWidget.h"

void UIPActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetActionVisual(InputAction);
}

void UIPActionWidget::SetAction(const UInputAction* InInputAction)
{
	InputAction = InInputAction;
}
