// Copyright pjdevs. All Rights Reserved.


#include "Flow/FlowNode_QuestObjective.h"
#include "QuestSubsystem.h"


static const FName StartObjectivePinName(TEXT("Start"));
static const FName SucceedObjectivePinName(TEXT("Succeed"));
static const FName FailObjectivePinName(TEXT("Fail"));
static const FName ObjectiveCompletedPinName(TEXT("Completed"));
static const FName ObjectiveStartedPinName(TEXT("Started"));

UFlowNode_QuestObjective::UFlowNode_QuestObjective(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
#endif
	InputPins = { FFlowPin(StartObjectivePinName), FFlowPin(SucceedObjectivePinName), FFlowPin(FailObjectivePinName) };
	OutputPins = { FFlowPin(ObjectiveCompletedPinName), FFlowPin(ObjectiveStartedPinName) };
}

void UFlowNode_QuestObjective::ExecuteInput(const FName& PinName)
{
	UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
	const FQuestId QuestId = QuestSubsystem->GetQuestIdFromFlow(GetFlowAsset());

	if (PinName == StartObjectivePinName)
	{
		ObjectiveCompletedDelegateHandle = QuestSubsystem->OnObjectiveCompleted.AddUObject(
			this,
			&UFlowNode_QuestObjective::OnObjectiveCompleted
		);

		QuestSubsystem->StartObjective(QuestId, ObjectiveRef.ObjectiveId);
		TriggerOutput(ObjectiveStartedPinName);
	}
	else if (PinName == SucceedObjectivePinName || PinName == FailObjectivePinName)
	{
		if (ObjectiveCompletedDelegateHandle.IsValid())
		{
			QuestSubsystem->OnObjectiveCompleted.Remove(ObjectiveCompletedDelegateHandle);
			ObjectiveCompletedDelegateHandle.Reset();
		}

		if (PinName == SucceedObjectivePinName)
		{
			QuestSubsystem->SucceedObjective(QuestId, ObjectiveRef.ObjectiveId);
		}
		else
		{
			QuestSubsystem->FailObjective(QuestId, ObjectiveRef.ObjectiveId);
		}

		TriggerOutput(ObjectiveCompletedPinName, true);
	}
}

void UFlowNode_QuestObjective::Cleanup()
{
	Super::Cleanup();

	if (ObjectiveCompletedDelegateHandle.IsValid())
	{
		UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
		QuestSubsystem->OnObjectiveCompleted.Remove(ObjectiveCompletedDelegateHandle);
		ObjectiveCompletedDelegateHandle.Reset();
	}
}

void UFlowNode_QuestObjective::OnLoad_Implementation()
{
	UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
	const FQuestId QuestId = QuestSubsystem->GetQuestIdFromFlow(GetFlowAsset());
	const FName& ObjectiveId = ObjectiveRef.ObjectiveId;
	
	if (QuestSubsystem->IsObjectiveCompleted(QuestId, ObjectiveId))
	{
		TriggerOutput(ObjectiveCompletedPinName, true);
	}
	else
	{
		ObjectiveCompletedDelegateHandle = QuestSubsystem->OnObjectiveCompleted.AddUObject(
			this,
			&UFlowNode_QuestObjective::OnObjectiveCompleted
		);
	}
}

UQuestSubsystem* UFlowNode_QuestObjective::GetQuestSubsystem() const
{
	const UWorld* World = GetWorld();
	ensureMsgf(World != nullptr, TEXT("World is nullptr in FlowNode_QuestObjective"));
	
	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	ensureMsgf(World != nullptr, TEXT("QuestSubsystem is nullptr in FlowNode_QuestObjective"));

	return QuestSubsystem;
}

void UFlowNode_QuestObjective::OnObjectiveCompleted(
	const FQuestId& QuestId,
	const FName& CompletedObjectiveId,
	EQuestObjectiveCompletionState CompletionState
)
{
	UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
	const FQuestId& ThisQuestId = QuestSubsystem->GetQuestIdFromFlow(GetFlowAsset());
	
	// for now assume unique tag per objective
	if (ThisQuestId == QuestId && CompletedObjectiveId == ObjectiveRef.ObjectiveId)
	{
		TriggerOutput(ObjectiveCompletedPinName, true);
	}
}

#if WITH_EDITOR
FString UFlowNode_QuestObjective::GetNodeDescription() const
{
	return ObjectiveRef.ObjectiveId.ToString();
}

EDataValidationResult UFlowNode_QuestObjective::ValidateNode()
{
	if (!ObjectiveRef.IsValid())
	{
		ValidationLog.Error<UFlowNode>(TEXT("ObjectiveRef is invalid or not referencing a quest objective."), this);
		return EDataValidationResult::Invalid;
	}

	return EDataValidationResult::Valid;
}
#endif
