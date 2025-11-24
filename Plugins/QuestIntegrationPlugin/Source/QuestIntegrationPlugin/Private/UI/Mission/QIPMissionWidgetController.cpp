// Copyright pjdevs. All Rights Reserved.


#include "UI/Mission/QIPMissionWidgetController.h"
#include "QuestComponent.h"
#include "GameFramework/GameStateBase.h"

void UQIPMissionWidgetController::InitializeWithGameState(AGameStateBase* InGameState)
{
	QuestComponent = InGameState->FindComponentByClass<UQuestComponent>();

	if (IsValid(QuestComponent))
	{
		TryUpdateFirstMission();
		QuestComponent->OnQuestUpdated.AddDynamic(this, &UQIPMissionWidgetController::OnQuestUpdated);
	}
}

void UQIPMissionWidgetController::Cleanup()
{
	if (IsValid(QuestComponent))
	{
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
	const TArray<FQuestDescription>& ActiveQuests = QuestComponent->GetActiveQuests();

	if (ActiveQuests.Num() > 0)
	{
		OnMissionUpdated.Broadcast(ActiveQuests[0]);
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
