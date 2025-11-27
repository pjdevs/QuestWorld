// Copyright pjdevs. All Rights Reserved.


#include "UI/Mission/QIPMissionWidgetController.h"

#include "QuestCompletionState.h"
#include "QuestComponent.h"
#include "GameFramework/GameStateBase.h"

void UQIPMissionWidgetController::InitializeWithGameState(AGameStateBase* InGameState)
{
	QuestComponent = InGameState->FindComponentByClass<UQuestComponent>();

	if (IsValid(QuestComponent))
	{
		TryUpdateFirstMission();
		QuestComponent->OnQuestStarted.AddDynamic(this, &UQIPMissionWidgetController::OnQuestUpdated);
		QuestComponent->OnQuestCompleted.AddDynamic(this, &UQIPMissionWidgetController::OnQuestUpdated);
		QuestComponent->OnQuestUpdated.AddDynamic(this, &UQIPMissionWidgetController::OnQuestUpdated);
	}
}

void UQIPMissionWidgetController::Cleanup()
{
	if (IsValid(QuestComponent))
	{
		QuestComponent->OnQuestStarted.RemoveDynamic(this, &UQIPMissionWidgetController::OnQuestUpdated);
		QuestComponent->OnQuestCompleted.RemoveDynamic(this, &UQIPMissionWidgetController::OnQuestUpdated);
		QuestComponent->OnQuestUpdated.RemoveDynamic(this, &UQIPMissionWidgetController::OnQuestUpdated);
	}
}

void UQIPMissionWidgetController::TriggerInitialEvents()
{
	if (IsValid(QuestComponent))
	{
		TryUpdateFirstMission();
	}
}

void UQIPMissionWidgetController::TryUpdateFirstMission() const
{
	const TArray<FQuestDescription>& Quests = QuestComponent->GetKnownQuests();

	if (Quests.Num() > 0 && Quests[0].CompletionState == EQuestCompletionState::Started)
	{
		OnMissionUpdated.Broadcast(Quests[0]);
	}
	else
	{
		OnNoMission.Broadcast();
	}
}

void UQIPMissionWidgetController::OnQuestUpdated(const FQuestDescription& QuestDescription)
{
	if (IsValid(QuestComponent))
	{
		TryUpdateFirstMission();
	}
}
