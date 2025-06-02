// Copyright pjdevs. All Rights Reserved.


#include "QuestIntegrationSubsystem.h"

#include "GameNotification.h"
#include "NotificationSubsystem.h"
#include "QuestIntegrationSettings.h"
#include "QuestSubsystem.h"

void UQuestIntegrationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if (!QuestIntegrationSettings.IsNull())
	{
		CurrentSettings = QuestIntegrationSettings.LoadSynchronous();
	}
	
	UQuestSubsystem* QuestSubsystem = GetGameInstance()->GetSubsystem<UQuestSubsystem>();

	QuestSubsystem->QuestStartedDelegate.AddDynamic(this, &UQuestIntegrationSubsystem::OnQuestStarted);
	QuestSubsystem->QuestCompletedDelegate.AddDynamic(this, &UQuestIntegrationSubsystem::OnQuestCompleted);
}

void UQuestIntegrationSubsystem::OnQuestStarted(const FQuestDescription& Quest)
{
	if (CurrentSettings == nullptr)
		return;
	
	UNotificationSubsystem* NotificationSubsystem = GetGameInstance()->GetSubsystem<UNotificationSubsystem>();

	FText Message = FText::Format(CurrentSettings->QuestStartedFormat, Quest.Title);
	
	NotificationSubsystem->QueueNotification(FGameNotification
	{
		CurrentSettings->QuestMessageWidgetClass,
		FName("Quest"),
		Message,
		0,
		CurrentSettings->QuestMessageDuration,
		nullptr,
		nullptr,
		nullptr
	});
}

void UQuestIntegrationSubsystem::OnQuestCompleted(const FQuestDescription& Quest)
{
	if (CurrentSettings == nullptr)
		return;

	UNotificationSubsystem* NotificationSubsystem = GetGameInstance()->GetSubsystem<UNotificationSubsystem>();

    NotificationSubsystem->QueueNotification(FGameNotification
    {
    	CurrentSettings->QuestMessageWidgetClass,
    	FName("Quest"),
    	FText::Format(CurrentSettings->QuestCompletedFormat, Quest.Title),
    	0,
    	CurrentSettings->QuestMessageDuration,
    	nullptr,
    	nullptr,
    	nullptr
    });
}
