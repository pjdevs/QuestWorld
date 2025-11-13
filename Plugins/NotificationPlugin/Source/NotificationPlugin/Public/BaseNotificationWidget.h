// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseNotificationWidget.generated.h"

DECLARE_DELEGATE(FNotificationDelegate);

struct FGameNotification;
/**
 * 
 */
UCLASS(Abstract)
class NOTIFICATIONPLUGIN_API UBaseNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeDestruct() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = Notification)
	void SetNotification(const FGameNotification& Notification);

	UFUNCTION(BlueprintCallable, Category = Notification)
	void NotificationEnded() const;

	FNotificationDelegate OnNotificationEnded;
	FNotificationDelegate OnNotificationDestroyed;
};
