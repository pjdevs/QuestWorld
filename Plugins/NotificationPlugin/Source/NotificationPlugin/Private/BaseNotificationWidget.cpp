// Copyright pjdevs. All Rights Reserved.


#include "BaseNotificationWidget.h"

void UBaseNotificationWidget::NotificationEnded() const
{
	bool _ = OnNotificationEnded.ExecuteIfBound();
}
