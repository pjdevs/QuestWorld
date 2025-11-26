// Copyright pjdevs. All Rights Reserved.


#include "Assets/QuestPhaseReference.h"

#include "Assets/QuestDataAsset.h"


#if WITH_EDITOR
void UQuestPhaseReference::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UQuestPhaseReference, Quest))
	{
		Phases.OnOwningQuestChanged(GetOwningQuest());
	}
}

UQuestDataAsset* UQuestPhaseReference::GetOwningQuest() const
{
	return Quest.LoadSynchronous();
}
#endif
