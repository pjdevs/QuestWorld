// Copyright pjdevs. All Rights Reserved.


#include "Assets/QuestPhaseReference.h"

#include "Assets/QuestDataAsset.h"


#if WITH_EDITOR
void FQuestPhaseReference::FixupReference()
{
	if (IsValid())
	{
		return;
	}

	if (QuestRef.ToSoftObjectPath().IsValid())
	{
		if (const UQuestDataAsset* QuestAsset = QuestRef.LoadSynchronous())
		{
			if (QuestAsset->Phases.Num() > 0)
			{
				Phase = *QuestAsset->Phases.begin();
				return;
			}
		}
	}

	QuestRef = nullptr;
	Phase = NAME_None;
}
	
bool FQuestPhaseReference::IsValid() const
{
	if (!QuestRef.ToSoftObjectPath().IsValid() || !Phase.IsValid() || Phase == NAME_None)
	{
		return false;
	}

	const UQuestDataAsset* QuestAsset = QuestRef.LoadSynchronous();

	if (QuestAsset == nullptr)
	{
		return false;
	}

	return QuestAsset->Phases.Contains(Phase);
}
#endif
