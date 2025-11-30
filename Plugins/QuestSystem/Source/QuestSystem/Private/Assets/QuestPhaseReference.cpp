// Copyright pjdevs. All Rights Reserved.


#include "Assets/QuestPhaseReference.h"

#include "Assets/QuestDataAsset.h"


#if WITH_EDITOR
bool UQuestPhaseReference::IsValid() const
{
	if (QuestRef.IsNull() || Phases.Phases.Num() != 1)
	{
		return false;
	}

	const UQuestDataAsset* QuestAsset = QuestRef.LoadSynchronous();
	
	for (const FName& Phase : Phases.Phases)
	{
		if (!QuestAsset->Phases.Contains(Phase))
		{
			return false;
		}
	}

	return true;
}

void UQuestPhaseReference::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UQuestPhaseReference, QuestRef))
	{
		Phases.OnOwningQuestChanged(GetOwningQuest());
	}
}

UQuestDataAsset* UQuestPhaseReference::GetOwningQuest() const
{
	return QuestRef.LoadSynchronous();
}
#endif
