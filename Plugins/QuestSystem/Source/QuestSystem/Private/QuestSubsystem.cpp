// Copyright pjdevs. All Rights Reserved.

#include "QuestSubsystem.h"
#include "QuestService.h"

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	QuestService = NewObject<UQuestServiceImpl>(this);
}

void UQuestSubsystem::Deinitialize()
{
	
}

void UQuestSubsystem::LoadQuests(const FQuestLoadedDynamicDelegate CompletionDelegate) const
{
	return QuestService->LoadQuests(FQuestLoadedDelegate::CreateLambda([CompletionDelegate]
	{
		bool _ = CompletionDelegate.ExecuteIfBound();
	}));
}

void UQuestSubsystem::StartQuest(const FName QuestId, APlayerController* PlayerController) const
{
	if (UWorld* World = GetWorld())
	{
		QuestService->StartQuest(QuestId, World, PlayerController);
	}
}

TArray<FName> UQuestSubsystem::GetActiveQuests() const
{
	return QuestService->GetActiveQuests();
}

TArray<FName> UQuestSubsystem::GetCompletedQuests() const
{
	return QuestService->GetCompletedQuests();
}
