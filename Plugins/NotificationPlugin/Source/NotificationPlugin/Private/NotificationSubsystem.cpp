// Copyright pjdevs. All Rights Reserved.


#include "NotificationSubsystem.h"
#include "BaseNotificationWidget.h"
#include "GameNotification.h"
#include "SpudSubsystem.h"
#include "Blueprint/UserWidget.h"


void UNotificationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	USpudSubsystem* SpudSubsystem = Collection.InitializeDependency<USpudSubsystem>();
	SpudSubsystem->AddPersistentGlobalObjectWithName(this, "NotificationSubsystem");
}

void UNotificationSubsystem::SpudPostRestore_Implementation(const USpudState* State)
{
	// empty subscribers on reload of notif, we will showing pending notifications on subscribe
	Subscribers.Empty();
}

void UNotificationSubsystem::QueueNotification(const FGameNotification& Message)
{
	QueuedMessages.Insert(Message, 0);
	TryDisplayNextNotification();
}

void UNotificationSubsystem::Subscribe(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		Subscribers.Add(PlayerController);
		TryDisplayNextNotification();
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
	if (bIsDisplaying || QueuedMessages.IsEmpty() || Subscribers.IsEmpty())
	{
		return;
	}

	const FGameNotification NextNotification = QueuedMessages.Pop();

	for (auto It = Subscribers.CreateIterator(); It; ++It)
	{
		if (It->IsValid())
		{
			APlayerController* Subscriber = It->Get();
			DisplayNotification(Subscriber, NextNotification);
		}
		else
		{
			It.RemoveCurrent();
		}
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
	NotificationWidget->OnNotificationDestroyed.BindUObject(this, &UNotificationSubsystem::OnNotificationDestroyed);
	NotificationWidget->AddToViewport();
	NotificationWidget->SetNotification(Notification);
}

void UNotificationSubsystem::OnNotificationEnded()
{
	if (IsValid(DisplayedWidget))
	{
		DisplayedWidget->OnNotificationEnded.Unbind();
		DisplayedWidget->OnNotificationDestroyed.Unbind();
		DisplayedWidget->RemoveFromParent();
		DisplayedWidget = nullptr;
	}

	bIsDisplaying = false;
	TryDisplayNextNotification();
}

void UNotificationSubsystem::OnNotificationDestroyed()
{
	DisplayedWidget = nullptr;
	bIsDisplaying = false;
}
