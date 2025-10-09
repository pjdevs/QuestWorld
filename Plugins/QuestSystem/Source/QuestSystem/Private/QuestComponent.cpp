// Copyright pjdevs. All Rights Reserved.


#include "QuestComponent.h"
#include "QuestService.h"


UQuestComponent::UQuestComponent()
	: QuestService(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestComponent::StartQuest(FPrimaryAssetId QuestId)
{
	QuestService->StartQuest(QuestId, GetWorld());
}

TArray<FQuestDescription> UQuestComponent::GetActiveQuests() const
{
	return QuestService->GetActiveQuestDescriptions();
}

TArray<FQuestDescription> UQuestComponent::GetCompletedQuests() const
{
	return QuestService->GetCompletedQuestDescriptions();
}

bool UQuestComponent::IsQuestCompleted(FPrimaryAssetId QuestId) const
{
	return QuestService->IsQuestCompleted(QuestId);
}

bool UQuestComponent::IsQuestActive(FPrimaryAssetId QuestId) const
{
	return QuestService->IsQuestActive(QuestId);
}

void UQuestComponent::SubmitQuestEvent(UBaseQuestEvent* Event)
{
	QuestService->SubmitQuestEvent(GetWorld(), Event);
}

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	QuestService = NewObject<UQuestServiceImpl>(this, FName("QuestService"));
	QuestService->SetQuestStartedDelegate(FQuestEventDelegate::CreateLambda(
	[this](const FQuestDescription& StartedQuest)
		{
			OnQuestStarted.Broadcast(StartedQuest);
		}
	));
	QuestService->SetQuestCompletedDelegate(FQuestEventDelegate::CreateLambda(
		[this](const FQuestDescription& CompletedQuest)
		{
			OnQuestCompleted.Broadcast(CompletedQuest);
		}
	));
	QuestService->LoadQuests(FQuestLoadedDelegate::CreateLambda(
		[this]
		{
			OnQuestsLoaded.Broadcast();
		}
	));
}
