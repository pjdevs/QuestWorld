// Copyright pjdevs. All Rights Reserved.


#include "ActiveQuest.h"

#include "ActiveQuestObjective.h"
#include "Assets/QuestDataAsset.h"


FActiveQuest::FActiveQuest(const FPrimaryAssetId& QuestId, UQuestDataAsset* QuestDataAsset, UWorld* World)
	: QuestId(QuestId), QuestDataAsset(QuestDataAsset), CurrentObjectiveIndex(0), bQuestCompleted(false)
{
	// TODO Activate objectives one by one
	
	for (auto& ObjectiveAsset : QuestDataAsset->GetQuestObjectives())
	{
		FActiveQuestObjective ActiveQuestObjective(ObjectiveAsset, World);
		Objectives.Add(ActiveQuestObjective);
	}
}

FActiveQuest::~FActiveQuest()
{
}

void FActiveQuest::OnQuestEvent(const FName& EventType, AActor* EventActor, UWorld* World)
{
	if (CurrentObjectiveIndex < 0 || CurrentObjectiveIndex >= Objectives.Num()) 
		return;
	
	while (CurrentObjectiveIndex < Objectives.Num())
	{
		FActiveQuestObjective& ActiveQuestObjective = Objectives[CurrentObjectiveIndex];
		ActiveQuestObjective.OnQuestEvent(EventType, EventActor, World);

		if (ActiveQuestObjective.IsObjectiveCompleted())
		{
			CurrentObjectiveIndex++;
		}
		else
		{
			break;
		}
	}

	if (CurrentObjectiveIndex >= Objectives.Num())
	{
		bQuestCompleted = true;
	}
}
