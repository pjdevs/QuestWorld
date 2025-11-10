// Copyright pjdevs. All Rights Reserved.


#include "UI/Mission/QIPMissionWidgetController.h"
#include "QuestComponent.h"
#include "GameFramework/GameStateBase.h"

void UQIPMissionWidgetController::InitializeWithGameState(AGameStateBase* InGameState)
{
	QuestComponent = InGameState->FindComponentByClass<UQuestComponent>();

	if (IsValid(QuestComponent))
	{
		OnActiveQuestsUpdated();
		QuestComponent->OnActiveQuestsUpdated.AddDynamic(this, &UQIPMissionWidgetController::OnActiveQuestsUpdated);
	}
}

void UQIPMissionWidgetController::Cleanup()
{
	if (IsValid(QuestComponent))
	{
		QuestComponent->OnActiveQuestsUpdated.RemoveDynamic(this, &UQIPMissionWidgetController::OnActiveQuestsUpdated);
	}
}

void UQIPMissionWidgetController::TriggerInitialEvents()
{
	if (IsValid(QuestComponent))
	{
		OnActiveQuestsUpdated();
	}
}

void UQIPMissionWidgetController::OnActiveQuestsUpdated()
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
