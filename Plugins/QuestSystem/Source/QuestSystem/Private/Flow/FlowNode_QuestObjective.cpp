// Copyright pjdevs. All Rights Reserved.


#include "Flow/FlowNode_QuestObjective.h"
#include "QuestSubsystem.h"


static const FName StartPinName(TEXT("Start"));
static const FName CompletePinName(TEXT("Complete"));

UFlowNode_QuestObjective::UFlowNode_QuestObjective(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
#endif
	InputPins = { FFlowPin(StartPinName), FFlowPin(CompletePinName) };
}

void UFlowNode_QuestObjective::ExecuteInput(const FName& PinName)
{
	const UWorld* World = GetWorld();
	ensureMsgf(World != nullptr, TEXT("World is nullptr in FlowNode_Quest"));
	
	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	ensureMsgf(World != nullptr, TEXT("QuestSubsystem is nullptr in FlowNode_Quest"));

	const FQuestId QuestId = QuestSubsystem->GetQuestIdFromFlow(GetFlowAsset());

	if (PinName == StartPinName)
	{
		QuestSubsystem->StartObjective(QuestId, ObjectiveId);
	}
	else if (PinName == CompletePinName)
	{
		QuestSubsystem->CompleteObjective(QuestId, ObjectiveId);
	}

	TriggerFirstOutput(true);
}

#if WITH_EDITOR
FString UFlowNode_QuestObjective::GetNodeDescription() const
{
	return ObjectiveId.ToString();
}

EDataValidationResult UFlowNode_QuestObjective::ValidateNode()
{
	if (!ObjectiveId.IsValid())
	{
		ValidationLog.Error<UFlowNode>(TEXT("ObjectiveId is invalid or not referencing a quest objective."), this);
		return EDataValidationResult::Invalid;
	}

	return EDataValidationResult::Valid;
}
#endif