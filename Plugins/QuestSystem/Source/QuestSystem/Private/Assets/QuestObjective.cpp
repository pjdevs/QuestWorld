// Copyright pjdevs. All Rights Reserved.


#include "Assets/QuestObjective.h"

#include "Assets/QuestDataAsset.h"

#if WITH_EDITOR
UQuestDataAsset* UQuestObjective::GetOwningQuest() const
{
	return Cast<UQuestDataAsset>(GetOuter());
}
#endif
