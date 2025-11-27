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

bool FQuestState::IsObjectiveCompleted(const FGameplayTag& ObjectiveId) const
{
	if (const FQuestObjectiveState* Objective = ObjectiveStates.Find(ObjectiveId))
	{
		return Objective->IsCompleted();
	}

	return false;
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
	FQuestObjectiveState& ObjectiveState = ObjectiveStates.Add(ObjectiveId, FQuestObjectiveState(ObjectiveAsset));
	
	if (ObjectiveState.IsRetroCompletable())
	{
		ObjectiveState.SetCurrentProgress(ObjectiveAsset->GetCompletion(World));

		if (ObjectiveState.IsCompleted())
		{
			ObjectiveState.SetCurrentProgress(ObjectiveAsset->GetTargetValue());
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
	OnObjectiveCompleted.ExecuteIfBound(ObjectiveId);
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

		if (ObjectiveState.IsCompleted())
		{
			CompleteObjective(ObjectiveId);
		}
	}

	return bAnyObjectiveProgressed;
}

void FQuestState::CompleteQuestIfAllObjectivesCompleted()
{
	if (!QuestAsset->bShouldAutocomplete)
	{
		return;
	}
	
	for (auto& [ObjectiveId, ObjectiveState] : ObjectiveStates)
	{
		if (!ObjectiveState.IsOptional() && !ObjectiveState.IsCompleted())
		{
			return;
		}
	}
	
	CompleteQuest();
}
