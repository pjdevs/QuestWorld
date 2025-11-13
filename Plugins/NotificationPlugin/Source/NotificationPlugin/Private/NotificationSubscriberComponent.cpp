// Copyright pjdevs. All Rights Reserved.


#include "NotificationSubscriberComponent.h"

#include "NotificationSubsystem.h"


UNotificationSubscriberComponent::UNotificationSubscriberComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNotificationSubscriberComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	if (!PlayerController ||!PlayerController->IsLocalController())
	{
		return;
	}

	UNotificationSubsystem* NotificationSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UNotificationSubsystem>();

	if (!NotificationSubsystem)
	{
		return;
	}

	NotificationSubsystem->Subscribe(PlayerController);
}

void UNotificationSubscriberComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	if (!PlayerController ||!PlayerController->IsLocalController())
	{
		return;
	}

	UNotificationSubsystem* NotificationSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UNotificationSubsystem>();

	if (!NotificationSubsystem)
	{
		return;
	}

	NotificationSubsystem->Unsubscribe(PlayerController);
}

