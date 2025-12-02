// Copyright pjdevs. All Rights Reserved.


#include "Flow/QuestFlowNode_StartQuestPhase.h"
#include "QuestSubsystem.h"
#include "Assets/QuestPhaseReference.h"


UQuestFlowNode_StartQuestPhase::UQuestFlowNode_StartQuestPhase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
#endif
}

void UQuestFlowNode_StartQuestPhase::ExecuteInput(const FName& PinName)
{
	const FQuestId& QuestId = GetOwningQuestId();
	UQuestSubsystem& QuestSubsystem = GetQuestSubsystem();

	if (PhaseRef)
	{
		QuestSubsystem.StartQuestPhase(QuestId, *PhaseRef->Phases.Phases.begin());
	}
	
	TriggerFirstOutput(true);
}

#if WITH_EDITOR
FString UQuestFlowNode_StartQuestPhase::GetNodeDescription() const
{
	return PhaseRef ? PhaseRef->Phases.ToString() : FString("None");
}

EDataValidationResult UQuestFlowNode_StartQuestPhase::ValidateNode()
{
	if (!PhaseRef || !PhaseRef->IsValid())
	{
		ValidationLog.Error<UFlowNode>(TEXT("ObjectiveRef is invalid or not referencing a quest objective."), this);
		return EDataValidationResult::Invalid;
	}

	return EDataValidationResult::Valid;
}
#endif