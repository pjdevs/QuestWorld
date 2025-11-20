// Copyright pjdevs. All Rights Reserved.


#include "ActiveQuest.h"
#include "ActiveQuestObjective.h"
#include "QuestSaveGame.h"
#include "Assets/QuestDataAsset.h"


FActiveQuest::FActiveQuest(const FPrimaryAssetId& QuestId, UQuestDataAsset* QuestDataAsset)
	: QuestDataAsset(QuestDataAsset), QuestId(QuestId), bQuestCompleted(false)
{
	for (TObjectPtr<UQuestObjective>& ObjectiveAsset : QuestDataAsset->Objectives)
	{
		ObjectiveAssets.Add(ObjectiveAsset->ObjectiveId, ObjectiveAsset);
	}
}

void FActiveQuest::StartObjective(const FGameplayTag& ObjectiveId, UWorld* World)
{
	TObjectPtr<UQuestObjective>* ObjectiveAssetPtr = ObjectiveAssets.Find(ObjectiveId);
	
	if (!ObjectiveAssetPtr)
	{
		return;
	}

	const TObjectPtr<UQuestObjective> ObjectiveAsset = *ObjectiveAssetPtr;
	FActiveQuestObjective& ActiveObjective = ActiveObjectives.Add(ObjectiveId, FActiveQuestObjective(ObjectiveAsset));
	
	if (ObjectiveAsset->bIsRetroCompletable)
	{
		ActiveObjective.SetCurrentProgress(ObjectiveAsset->GetCompletion(World));

		if (ActiveObjective.IsObjectiveCompleted())
		{
			CompleteObjective(ObjectiveId);
		}
	}
}

void FActiveQuest::CompleteObjective(const FGameplayTag& ObjectiveId)
{
	FActiveQuestObjective* Objective = ActiveObjectives.Find(ObjectiveId);

	if (!Objective)
	{
		return;
	}

	Objective->CompleteObjective();
	
	ActiveObjectives.Remove(ObjectiveId);
	CompletedObjectives.Add(ObjectiveId);
	
	bool bAreAllObjectivesCompleted = CompletedObjectives.Num() == ObjectiveAssets.Num();

	if (bAreAllObjectivesCompleted && QuestDataAsset->bShouldAutocomplete)
	{
		bQuestCompleted = true;
	}
}

void FActiveQuest::ProgressObjective(const FGameplayTag& ObjectiveId, int Progress)
{
	FActiveQuestObjective* Objective = ActiveObjectives.Find(ObjectiveId);

	if (!Objective)
	{
		return;
	}

	Objective->ProgressObjective(Progress);
}

void FActiveQuest::CompleteQuest()
{
	bQuestCompleted = true;
}

bool FActiveQuest::OnQuestEvent(UWorld* World, UBaseQuestEvent* Event)
{
	if (bQuestCompleted)
	{
		return false;
	}
	
	bool bAnyObjectiveProgressed = false;
	bool bAllObjectivesCompleted = true;

	for (auto It = ActiveObjectives.CreateIterator(); It; ++It)
	{
		FActiveQuestObjective& ActiveObjective = It.Value();
		
		if (ActiveObjective.IsObjectiveCompleted())
		{
			continue;
		}
		
		bAnyObjectiveProgressed |= ActiveObjective.OnQuestEvent(World, Event);
		bAllObjectivesCompleted &= ActiveObjective.IsObjectiveCompleted();
	}

	if (QuestDataAsset->bShouldAutocomplete && bAllObjectivesCompleted)
	{
		CompleteQuest();
	}

	return bAnyObjectiveProgressed;
}
