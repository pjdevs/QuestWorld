// Copyright pjdevs. All Rights Reserved.


#include "NotificationSubsystem.h"

#include "BaseNotificationWidget.h"
#include "GameNotification.h"
#include "Blueprint/UserWidget.h"


void UNotificationSubsystem::QueueNotification(const FGameNotification& Message)
{
	QueuedMessages.Enqueue(Message);

	if (!bIsDisplaying)
	{
		TryDisplayNextNotification();
	}
}

void UNotificationSubsystem::Subscribe(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		Subscribers.Add(PlayerController);
	}
}

void UNotificationSubsystem::Unsubscribe(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		Subscribers.Remove(PlayerController);
	}
}

void UNotificationSubsystem::TryDisplayNextNotification()
{
	FGameNotification NextNotification;

	if (!QueuedMessages.Dequeue(NextNotification))
		return;
	
	for (TWeakObjectPtr<APlayerController> WeakSubscriber : Subscribers)
	{
		APlayerController* Subscriber = WeakSubscriber.Get();
		
		if (Subscriber == nullptr || !IsValid(Subscriber))
			continue;
		
		DisplayNotification(Subscriber, NextNotification);
	}
}

void UNotificationSubsystem::DisplayNotification(
	APlayerController* PlayerController,
	const FGameNotification& Notification
)
{
	 UBaseNotificationWidget* NotificationWidget = Cast<UBaseNotificationWidget>(
	 	CreateWidget(PlayerController, Notification.NotificationWidgetClass.Get())
	);

	if (NotificationWidget == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Cannot create Notification Widget"));
		OnNotificationEnded();
		return;
	}
	
	DisplayedWidget = NotificationWidget;
	bIsDisplaying = true;
	
	NotificationWidget->OnNotificationEnded.BindUObject(this, &UNotificationSubsystem::OnNotificationEnded);
	NotificationWidget->AddToViewport();
	NotificationWidget->SetNotification(Notification);
}

void UNotificationSubsystem::OnNotificationEnded()
{
	if (DisplayedWidget != nullptr && IsValid(DisplayedWidget))
	{
		DisplayedWidget->RemoveFromParent();
		DisplayedWidget = nullptr;
	}

	bIsDisplaying = false;

	TryDisplayNextNotification();
}
