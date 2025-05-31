// Copyright pjdevs. All Rights Reserved.

#include "QuestSubsystem.h"

#include "QuestDescription.h"
#include "QuestService.h"

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	QuestService = NewObject<UQuestServiceImpl>(this);
	QuestService->SetQuestStartedDelegate(FQuestEventDelegate::CreateLambda(
	[this](const FQuestDescription& StartedQuest)
		{
			QuestStartedDelegate.Broadcast(StartedQuest);
		}
	));
	QuestService->SetQuestCompletedDelegate(FQuestEventDelegate::CreateLambda(
		[this](const FQuestDescription& CompletedQuest)
		{
			QuestCompletedDelegate.Broadcast(CompletedQuest);
		}
	));
}

void UQuestSubsystem::Deinitialize()
{
	
}

void UQuestSubsystem::LoadQuests(const FQuestLoadedDynamicDelegate CompletionDelegate)
{
	return QuestService->LoadQuests(FQuestLoadedDelegate::CreateLambda([CompletionDelegate]
	{
		bool _ = CompletionDelegate.ExecuteIfBound();
	}));
}

void UQuestSubsystem::StartQuest(const FPrimaryAssetId& QuestId)
{
	if (UWorld* World = GetWorld())
	{
		QuestService->StartQuest(QuestId, World);
	}
}

TArray<FQuestDescription> UQuestSubsystem::GetActiveQuests() const
{
	return QuestService->GetActiveQuestDescriptions();
}

TArray<FQuestDescription> UQuestSubsystem::GetCompletedQuests() const
{
	return QuestService->GetCompletedQuestDescriptions();
}

void UQuestSubsystem::SubmitQuestEvent(UBaseQuestEvent* Event)
{
	if (UWorld* World = GetWorld())
	{
		QuestService->SubmitQuestEvent(World, Event);
	}
}
