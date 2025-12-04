// Copyright pjdevs. All Rights Reserved.


#include "Assets/QuestObjectiveReference.h"

#include "Assets/QuestDataAsset.h"


#if WITH_EDITOR
void FQuestObjectiveReference::FixupReference()
{
	if (IsValid())
	{
		return;
	}

	if (QuestRef.ToSoftObjectPath().IsValid())
	{
		if (const UQuestDataAsset* QuestAsset = QuestRef.LoadSynchronous())
		{
			if (QuestAsset->Objectives.Num() > 0)
			{
				ObjectiveId = QuestAsset->Objectives[0]->ObjectiveId;
				return;
			}
		}
	}

	QuestRef = nullptr;
	ObjectiveId = NAME_None;
}
	
bool FQuestObjectiveReference::IsValid() const
{
	if (!QuestRef.ToSoftObjectPath().IsValid() || !ObjectiveId.IsValid() || ObjectiveId == NAME_None)
	{
		return false;
	}

	const UQuestDataAsset* QuestAsset = QuestRef.LoadSynchronous();

	if (QuestAsset == nullptr)
	{
		return false;
	}

	return QuestAsset->Objectives.ContainsByPredicate(
		[&](const TObjectPtr<UQuestObjective>& Objective) { return Objective->ObjectiveId == ObjectiveId; }
	);
}
#endif
