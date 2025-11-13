// Copyright pjdevs. All Rights Reserved.


#include "BaseNotificationWidget.h"

void UBaseNotificationWidget::NativeDestruct()
{
	Super::NativeDestruct();

	OnNotificationDestroyed.ExecuteIfBound();
}

void UBaseNotificationWidget::NotificationEnded() const
{
	OnNotificationEnded.ExecuteIfBound();
}
