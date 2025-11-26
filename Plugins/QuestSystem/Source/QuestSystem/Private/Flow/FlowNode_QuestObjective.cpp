// Copyright pjdevs. All Rights Reserved.


#include "Flow/FlowNode_QuestObjective.h"
#include "QuestSubsystem.h"


static const FName StartPinName(TEXT("Start"));
static const FName CompletePinName(TEXT("Complete"));
static const FName CompletedPinName(TEXT("Completed"));
static const FName StartedPinName(TEXT("Started"));

UFlowNode_QuestObjective::UFlowNode_QuestObjective(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
#endif
	InputPins = { FFlowPin(StartPinName), FFlowPin(CompletePinName) };
	OutputPins = { FFlowPin(CompletedPinName), FFlowPin(StartedPinName) };
}

void UFlowNode_QuestObjective::ExecuteInput(const FName& PinName)
{
	UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
	const FQuestId QuestId = QuestSubsystem->GetQuestIdFromFlow(GetFlowAsset());

	ObjectiveCompletedDelegateHandle = QuestSubsystem->OnObjectiveCompleted.AddUObject(
		this,
		&UFlowNode_QuestObjective::OnObjectiveCompleted
	);

	if (PinName == StartPinName)
	{
		QuestSubsystem->StartObjective(QuestId, ObjectiveId);
		TriggerOutput(StartedPinName);
	}
	else if (PinName == CompletePinName)
	{
		QuestSubsystem->OnObjectiveCompleted.Remove(ObjectiveCompletedDelegateHandle);
		ObjectiveCompletedDelegateHandle.Reset();
		QuestSubsystem->CompleteObjective(QuestId, ObjectiveId);
		TriggerOutput(CompletePinName, true);
	}
}

void UFlowNode_QuestObjective::Cleanup()
{
	Super::Cleanup();

	if (ObjectiveCompletedDelegateHandle.IsValid())
	{
		UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
		QuestSubsystem->OnObjectiveCompleted.Remove(ObjectiveCompletedDelegateHandle);
	}
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

UQuestSubsystem* UFlowNode_QuestObjective::GetQuestSubsystem() const
{
	const UWorld* World = GetWorld();
	ensureMsgf(World != nullptr, TEXT("World is nullptr in FlowNode_QuestObjective"));
	
	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	ensureMsgf(World != nullptr, TEXT("QuestSubsystem is nullptr in FlowNode_QuestObjective"));

	return QuestSubsystem;
}

void UFlowNode_QuestObjective::OnObjectiveCompleted(const FQuestId& QuestId, const FGameplayTag& CompletedObjectiveId)
{
	// for now assume unique tag per objective
	if (CompletedObjectiveId == ObjectiveId)
	{
		TriggerOutput(CompletedPinName, true);
	}
}
#endif
