// Copyright pjdevs. All Rights Reserved.


#include "Quest/QuestNotificationComponent.h"

#include "GameNotification.h"
#include "NotificationSubsystem.h"
#include "QuestComponent.h"
#include "QuestDescription.h"
#include "QuestStatics.h"
#include "Quest/QuestNotificationSettings.h"


UQuestNotificationComponent::UQuestNotificationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestNotificationComponent::BeginPlay()
{
	Super::BeginPlay();

	const APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	if (!PlayerController ||!PlayerController->IsLocalController())
	{
		return;
	}

	if (!IsValid(QuestNotificationSettings))
	{
		QuestNotificationSettings = NewObject<UQuestNotificationSettings>(this);
	}
	
	if (UQuestComponent* QuestComponent = UQuestStatics::GetQuestComponent(GetWorld()))
	{
		QuestComponent->OnQuestStarted.AddDynamic(this, &UQuestNotificationComponent::OnQuestStarted);
		QuestComponent->OnQuestCompleted.AddDynamic(this, &UQuestNotificationComponent::OnQuestCompleted);
		QuestComponent->OnQuestUpdated.AddDynamic(this, &UQuestNotificationComponent::OnQuestUpdated);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Quest Component is NULL. Can't initialize quest notifications."));
	}
}

void UQuestNotificationComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UQuestComponent* QuestComponent = UQuestStatics::GetQuestComponent(GetWorld()))
	{
		QuestComponent->OnQuestStarted.RemoveDynamic(this, &UQuestNotificationComponent::OnQuestStarted);
		QuestComponent->OnQuestCompleted.RemoveDynamic(this, &UQuestNotificationComponent::OnQuestCompleted);
		QuestComponent->OnQuestUpdated.RemoveDynamic(this, &UQuestNotificationComponent::OnQuestUpdated);
	}
}

void UQuestNotificationComponent::OnQuestStarted(const FQuestDescription& Quest)
{
	UNotificationSubsystem* NotificationSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UNotificationSubsystem>();

	if (!NotificationSubsystem)
	{
		return;
	}
	
	NotificationSubsystem->QueueNotification(FGameNotification
	{
		QuestNotificationSettings->QuestMessageWidgetClass,
		FName("Quest"),
		FText::Format(QuestNotificationSettings->QuestStartedFormat, Quest.Title),
		0,
		QuestNotificationSettings->QuestMessageDuration,
		nullptr,
		nullptr,
		nullptr
	});
}

void UQuestNotificationComponent::OnQuestCompleted(const FQuestDescription& Quest)
{
	UNotificationSubsystem* NotificationSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UNotificationSubsystem>();

	if (!NotificationSubsystem)
	{
		return;
	}
		
	NotificationSubsystem->QueueNotification(FGameNotification
	{
		QuestNotificationSettings->QuestMessageWidgetClass,
		FName("Quest"),
		FText::Format(QuestNotificationSettings->QuestCompletedFormat, Quest.Title),
		0,
		QuestNotificationSettings->QuestMessageDuration,
		nullptr,
		nullptr,
		nullptr
	});
}

void UQuestNotificationComponent::OnQuestUpdated(const FQuestDescription& Quest)
{
	UNotificationSubsystem* NotificationSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UNotificationSubsystem>();

	if (!NotificationSubsystem)
	{
		return;
	}
	
	NotificationSubsystem->QueueNotification(FGameNotification
	{
		QuestNotificationSettings->QuestMessageWidgetClass,
		FName("Quest"),
		FText::Format(QuestNotificationSettings->QuestUpdatedFormat, Quest.Title),
		0,
		QuestNotificationSettings->QuestMessageDuration,
		nullptr,
		nullptr,
		nullptr
	});
}
