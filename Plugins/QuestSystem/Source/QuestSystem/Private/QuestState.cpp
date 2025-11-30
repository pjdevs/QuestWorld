// Copyright pjdevs. All Rights Reserved.


#include "QuestState.h"
#include "QuestObjectiveState.h"
#include "QuestSave.h"
#include "Assets/QuestDataAsset.h"


FQuestState::FQuestState(const FQuestId& QuestId, const UQuestDataAsset* QuestAsset)
	: QuestId(QuestId), QuestAsset(QuestAsset), State(EQuestCompletionState::Started)
{
	for (const TObjectPtr<UQuestObjective>& ObjectiveAsset : QuestAsset->Objectives)
	{
		ObjectiveAssets.Add(ObjectiveAsset->ObjectiveId, ObjectiveAsset);
	}
}

bool FQuestState::IsObjectiveCompleted(const FName& ObjectiveId) const
{
	if (const FQuestObjectiveState* Objective = ObjectiveStates.Find(ObjectiveId))
	{
		return Objective->IsCompleted();
	}

	return false;
}

bool FQuestState::IsObjectiveSucceeded(const FName& ObjectiveId) const
{
	if (const FQuestObjectiveState* Objective = ObjectiveStates.Find(ObjectiveId))
	{
		return Objective->GetCompletionState() == EQuestObjectiveCompletionState::Succeeded;
	}

	return false;
}

bool FQuestState::IsObjectiveFailed(const FName& ObjectiveId) const
{
	if (const FQuestObjectiveState* Objective = ObjectiveStates.Find(ObjectiveId))
	{
		return Objective->GetCompletionState() == EQuestObjectiveCompletionState::Failed;
	}

	 return false;
}

const FName& FQuestState::GetActivePhase() const
{
	return ActivePhase;
}

const TArray<FName>& FQuestState::GetCompletedPhases() const
{
	return CompletedPhases;
}

void FQuestState::StartPhase(const FName& Phase)
{
	if (ActivePhase == NAME_None && !CompletedPhases.Contains(Phase))
	{
		ActivePhase = Phase;
	}
}

void FQuestState::CompleteActivePhase()
{
	CompletedPhases.Add(ActivePhase);
	ActivePhase = NAME_None;
}

void FQuestState::RestoreCompletedPhases(const TArray<FName>& InCompletedPhases)
{
	CompletedPhases = InCompletedPhases;
}

void FQuestState::StartObjective(const FName& ObjectiveId, UWorld* World)
{
	if (IsCompleted())
	{
		return;
	}

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
			HandleObjectiveCompletion(ObjectiveState);
		}
	}
}

void FQuestState::CompleteObjective(const FName& ObjectiveId, EQuestObjectiveCompletionState CompletionState)
{
	if (IsCompleted())
	{
		return;
	}

	ensure(CompletionState != EQuestObjectiveCompletionState::Started);

	FQuestObjectiveState* ObjectiveState = ObjectiveStates.Find(ObjectiveId);

	// Make a new objective state if it has not been started already
	if (!ObjectiveState)
	{
		const TObjectPtr<const UQuestObjective>* ObjectiveAssetPtr = ObjectiveAssets.Find(ObjectiveId);

		if (!ObjectiveAssetPtr)
		{
			UE_LOG(LogTemp, Display, TEXT("Could not complete objective %s. Asset not found."), *ObjectiveId.ToString());
			return;
		}

		const TObjectPtr<const UQuestObjective>& ObjectiveAsset = *ObjectiveAssetPtr;
		ObjectiveState = &ObjectiveStates.Add(ObjectiveId, FQuestObjectiveState(ObjectiveAsset));
	}

	ObjectiveState->SetCompletionState(CompletionState);
	HandleObjectiveCompletion(*ObjectiveState);
}

void FQuestState::ProgressObjective(const FName& ObjectiveId, int Progress)
{
	if (IsCompleted())
	{
		return;
	}

	FQuestObjectiveState* Objective = ObjectiveStates.Find(ObjectiveId);

	if (!Objective)
	{
		UE_LOG(LogTemp, Display, TEXT("Could not progress objective %s. Not found."), *ObjectiveId.ToString());
		return;
	}

	Objective->ProgressObjective(Progress);
}

void FQuestState::SetCompletionState(EQuestCompletionState CompletionState)
{
	State = CompletionState;
}

bool FQuestState::OnQuestEvent(UWorld* World, UBaseQuestEvent* Event)
{
	if (IsCompleted())
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
			CompleteObjective(ObjectiveId, EQuestObjectiveCompletionState::Succeeded);
		}
	}

	return bAnyObjectiveProgressed;
}

void FQuestState::HandleObjectiveCompletion(const FQuestObjectiveState& ObjectiveState)
{
	OnObjectiveCompleted.ExecuteIfBound(ObjectiveState.GetObjectiveId(), ObjectiveState.GetCompletionState());

	if (ObjectiveState.GetCompletionState() == EQuestObjectiveCompletionState::Succeeded)
	{
		CompleteQuestIfAllObjectivesCompleted();
	}
}

void FQuestState::CompleteQuestIfAllObjectivesCompleted()
{
	if (IsCompleted())
	{
		return;
	}

	if (!QuestAsset->bShouldAutocomplete)
	{
		return;
	}
	
	for (auto& [ObjectiveId, ObjectiveState] : ObjectiveStates)
	{
		if (!ObjectiveState.IsOptional() && ObjectiveState.GetCompletionState() == EQuestObjectiveCompletionState::Succeeded)
		{
			return;
		}
	}
	
	SetCompletionState(EQuestCompletionState::Succeeded);
}
