// Copyright pjdevs. All Rights Reserved.


#include "QuestIntegrationSubsystem.h"

#include "GameNotification.h"
#include "NotificationSubsystem.h"
#include "QuestIntegrationSettings.h"
#include "QuestSubsystem.h"

void UQuestIntegrationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UQuestSubsystem* QuestSubsystem = GetGameInstance()->GetSubsystem<UQuestSubsystem>();

	QuestSubsystem->QuestStartedDelegate.AddDynamic(this, &UQuestIntegrationSubsystem::OnQuestStarted);
	QuestSubsystem->QuestCompletedDelegate.AddDynamic(this, &UQuestIntegrationSubsystem::OnQuestCompleted);
}

void UQuestIntegrationSubsystem::SetQuestIntegrationSettings(UQuestIntegrationSettings* Settings)
{
	QuestIntegrationSettings = Settings;
}

void UQuestIntegrationSubsystem::OnQuestStarted(const FQuestDescription& Quest)
{
	if (QuestIntegrationSettings == nullptr)
		return;
	
	UNotificationSubsystem* NotificationSubsystem = GetGameInstance()->GetSubsystem<UNotificationSubsystem>();

	FText Message = FText::Format(QuestIntegrationSettings->QuestStartedFormat, Quest.Title);
	
	NotificationSubsystem->QueueNotification(FGameNotification
	{
		QuestIntegrationSettings->QuestMessageWidgetClass,
		FName("Quest"),
		Message,
		0,
		QuestIntegrationSettings->QuestMessageDuration,
		nullptr,
		nullptr,
		nullptr
	});
}

void UQuestIntegrationSubsystem::OnQuestCompleted(const FQuestDescription& Quest)
{
	if (QuestIntegrationSettings == nullptr)
		return;

	UNotificationSubsystem* NotificationSubsystem = GetGameInstance()->GetSubsystem<UNotificationSubsystem>();

    NotificationSubsystem->QueueNotification(FGameNotification
    {
    	QuestIntegrationSettings->QuestMessageWidgetClass,
    	FName("Quest"),
    	FText::Format(QuestIntegrationSettings->QuestCompletedFormat, Quest.Title),
    	0,
    	QuestIntegrationSettings->QuestMessageDuration,
    	nullptr,
    	nullptr,
    	nullptr
    });
}
