// Copyright pjdevs. All Rights Reserved.


#include "QuestState.h"
#include "QuestObjectiveState.h"
#include "QuestSave.h"
#include "Assets/QuestDataAsset.h"


FQuestState::FQuestState(const FQuestId& QuestId, const UQuestDataAsset* QuestAsset)
	: QuestId(QuestId), QuestAsset(QuestAsset), bQuestCompleted(false)
{
	for (const TObjectPtr<UQuestObjective>& ObjectiveAsset : QuestAsset->Objectives)
	{
		ObjectiveAssets.Add(ObjectiveAsset->ObjectiveId, ObjectiveAsset);
	}
}

void FQuestState::StartObjective(const FGameplayTag& ObjectiveId, UWorld* World)
{
	const TObjectPtr<const UQuestObjective>* ObjectiveAssetPtr = ObjectiveAssets.Find(ObjectiveId);
	
	if (!ObjectiveAssetPtr)
	{
		UE_LOG(LogTemp, Display, TEXT("Could not start objective %s. Asset not found."), *ObjectiveId.ToString());
		return;
	}

	const TObjectPtr<const UQuestObjective>& ObjectiveAsset = *ObjectiveAssetPtr;
	FQuestObjectiveState& ActiveObjective = ObjectiveStates.Add(ObjectiveId, FQuestObjectiveState(ObjectiveAsset));
	
	if (ActiveObjective.IsRetroCompletable())
	{
		ActiveObjective.SetCurrentProgress(ObjectiveAsset->GetCompletion(World));

		if (ActiveObjective.IsCompleted())
		{
			CompleteObjective(ObjectiveId);
		}
	}
}

void FQuestState::CompleteObjective(const FGameplayTag& ObjectiveId)
{
	FQuestObjectiveState* Objective = ObjectiveStates.Find(ObjectiveId);

	if (!Objective)
	{
		const TObjectPtr<const UQuestObjective>* ObjectiveAssetPtr = ObjectiveAssets.Find(ObjectiveId);

		if (!ObjectiveAssetPtr)
		{
			UE_LOG(LogTemp, Display, TEXT("Could not complete objective %s. Asset not found."), *ObjectiveId.ToString());
			return;
		}

		const TObjectPtr<const UQuestObjective>& ObjectiveAsset = *ObjectiveAssetPtr;
		Objective = &ObjectiveStates.Add(ObjectiveId, FQuestObjectiveState(ObjectiveAsset));
	}

	Objective->CompleteObjective();
	CompleteQuestIfAllObjectivesCompleted();
}

void FQuestState::ProgressObjective(const FGameplayTag& ObjectiveId, int Progress)
{
	FQuestObjectiveState* Objective = ObjectiveStates.Find(ObjectiveId);

	if (!Objective)
	{
		UE_LOG(LogTemp, Display, TEXT("Could not progress objective %s. Not found."), *ObjectiveId.ToString());
		return;
	}

	Objective->ProgressObjective(Progress);
}

void FQuestState::CompleteQuest()
{
	bQuestCompleted = true;
}

bool FQuestState::OnQuestEvent(UWorld* World, UBaseQuestEvent* Event)
{
	if (bQuestCompleted)
	{
		return false;
	}
	
	bool bAnyObjectiveProgressed = false;

	for (auto& [ObjectiveId, ObjectiveState] : ObjectiveStates)
	{
		if (ObjectiveState.IsCompleted())
		{
			continue;
		}
		
		bAnyObjectiveProgressed |= ObjectiveState.OnQuestEvent(World, Event);
	}

	CompleteQuestIfAllObjectivesCompleted();

	return bAnyObjectiveProgressed;
}

void FQuestState::CompleteQuestIfAllObjectivesCompleted()
{
	if (!QuestAsset->bShouldAutocomplete)
	{
		return;
	}
	
	if (ObjectiveStates.Num() == ObjectiveAssets.Num())
	{
		for (auto& [ObjectiveId, ObjectiveState] : ObjectiveStates)
		{
			if (!ObjectiveState.IsCompleted())
			{
				return;
			}
		}
		
		CompleteQuest();
	}
}
