// Copyright pjdevs. All Rights Reserved.


#include "Flow/FlowNode_StartQuestPhase.h"
#include "QuestSubsystem.h"
#include "Assets/QuestPhaseReference.h"


UFlowNode_StartQuestPhase::UFlowNode_StartQuestPhase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
#endif
}

void UFlowNode_StartQuestPhase::ExecuteInput(const FName& PinName)
{
	UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
	const FQuestId QuestId = QuestSubsystem->GetQuestIdFromFlow(GetFlowAsset());

	if (PhaseRef)
	{
		QuestSubsystem->StartQuestPhase(QuestId, *PhaseRef->Phases.Phases.begin());
	}
	
	TriggerFirstOutput(true);
}

UQuestSubsystem* UFlowNode_StartQuestPhase::GetQuestSubsystem() const
{
	const UWorld* World = GetWorld();
	ensureMsgf(World != nullptr, TEXT("World is nullptr in UFlowNode_StartQuestPhase"));
	
	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	ensureMsgf(World != nullptr, TEXT("QuestSubsystem is nullptr in UFlowNode_StartQuestPhase"));

	return QuestSubsystem;
}

#if WITH_EDITOR
FString UFlowNode_StartQuestPhase::GetNodeDescription() const
{
	return PhaseRef ? PhaseRef->Phases.ToString() : FString("None");
}

EDataValidationResult UFlowNode_StartQuestPhase::ValidateNode()
{
	if (!PhaseRef || !PhaseRef->IsValid())
	{
		ValidationLog.Error<UFlowNode>(TEXT("ObjectiveRef is invalid or not referencing a quest objective."), this);
		return EDataValidationResult::Invalid;
	}

	return EDataValidationResult::Valid;
}
#endif