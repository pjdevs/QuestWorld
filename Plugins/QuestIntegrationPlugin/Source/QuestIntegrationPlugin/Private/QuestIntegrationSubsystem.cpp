// Copyright pjdevs. All Rights Reserved.


#include "QuestIntegrationSubsystem.h"

#include "GameNotification.h"
#include "NotificationSubsystem.h"
#include "QuestComponent.h"
#include "QuestIntegrationSettings.h"
#include "QuestStatics.h"


void UQuestIntegrationSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (!QuestIntegrationSettings.IsNull())
	{
		CurrentSettings = QuestIntegrationSettings.LoadSynchronous();
	}
	
	UQuestComponent* QuestSubsystem = UQuestStatics::GetQuestComponent(GetWorld());

	QuestSubsystem->OnQuestStarted.AddDynamic(this, &UQuestIntegrationSubsystem::OnQuestStarted);
	QuestSubsystem->OnQuestCompleted.AddDynamic(this, &UQuestIntegrationSubsystem::OnQuestCompleted);
}

void UQuestIntegrationSubsystem::OnQuestStarted(const FQuestDescription& Quest)
{
	if (CurrentSettings == nullptr)
		return;
	
	UNotificationSubsystem* NotificationSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UNotificationSubsystem>();

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

	UNotificationSubsystem* NotificationSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UNotificationSubsystem>();

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
