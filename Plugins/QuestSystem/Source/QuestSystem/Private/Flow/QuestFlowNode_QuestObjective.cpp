// Copyright pjdevs. All Rights Reserved.


#include "Flow/QuestFlowNode_QuestObjective.h"
#include "QuestSubsystem.h"


static const FName StartObjectivePinName(TEXT("Start"));
static const FName SucceedObjectivePinName(TEXT("Succeed"));
static const FName FailObjectivePinName(TEXT("Fail"));
static const FName ObjectiveCompletedPinName(TEXT("Completed"));
static const FName ObjectiveStartedPinName(TEXT("Started"));

UQuestFlowNode_QuestObjective::UQuestFlowNode_QuestObjective(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
#endif
	InputPins = { FFlowPin(StartObjectivePinName), FFlowPin(SucceedObjectivePinName), FFlowPin(FailObjectivePinName) };
	OutputPins = { FFlowPin(ObjectiveCompletedPinName), FFlowPin(ObjectiveStartedPinName) };
}

void UQuestFlowNode_QuestObjective::ExecuteInput(const FName& PinName)
{
	UQuestSubsystem& QuestSubsystem = GetQuestSubsystem();
	const FQuestId QuestId = QuestSubsystem.GetQuestIdFromFlow(GetFlowAsset());

	if (PinName == StartObjectivePinName)
	{
		ObjectiveCompletedDelegateHandle = QuestSubsystem.OnObjectiveCompleted.AddUObject(
			this,
			&UQuestFlowNode_QuestObjective::OnObjectiveCompleted
		);

		QuestSubsystem.StartObjective(QuestId, ObjectiveRef.ObjectiveId);
		TriggerOutput(ObjectiveStartedPinName);
	}
	else if (PinName == SucceedObjectivePinName || PinName == FailObjectivePinName)
	{
		if (ObjectiveCompletedDelegateHandle.IsValid())
		{
			QuestSubsystem.OnObjectiveCompleted.Remove(ObjectiveCompletedDelegateHandle);
			ObjectiveCompletedDelegateHandle.Reset();
		}

		if (PinName == SucceedObjectivePinName)
		{
			QuestSubsystem.SucceedObjective(QuestId, ObjectiveRef.ObjectiveId);
		}
		else
		{
			QuestSubsystem.FailObjective(QuestId, ObjectiveRef.ObjectiveId);
		}

		TriggerOutput(ObjectiveCompletedPinName, true);
	}
}

void UQuestFlowNode_QuestObjective::Cleanup()
{
	Super::Cleanup();

	if (ObjectiveCompletedDelegateHandle.IsValid())
	{
		UQuestSubsystem& QuestSubsystem = GetQuestSubsystem();
		QuestSubsystem.OnObjectiveCompleted.Remove(ObjectiveCompletedDelegateHandle);
		ObjectiveCompletedDelegateHandle.Reset();
	}
}

void UQuestFlowNode_QuestObjective::OnLoad_Implementation()
{
	UQuestSubsystem& QuestSubsystem = GetQuestSubsystem();
	const FQuestId QuestId = QuestSubsystem.GetQuestIdFromFlow(GetFlowAsset());
	const FName& ObjectiveId = ObjectiveRef.ObjectiveId;
	
	if (QuestSubsystem.IsObjectiveCompleted(QuestId, ObjectiveId))
	{
		TriggerOutput(ObjectiveCompletedPinName, true);
	}
	else
	{
		ObjectiveCompletedDelegateHandle = QuestSubsystem.OnObjectiveCompleted.AddUObject(
			this,
			&UQuestFlowNode_QuestObjective::OnObjectiveCompleted
		);
	}
}

void UQuestFlowNode_QuestObjective::OnObjectiveCompleted(
	const FQuestId& QuestId,
	const FName& CompletedObjectiveId,
	EQuestObjectiveCompletionState CompletionState
)
{
	UQuestSubsystem& QuestSubsystem = GetQuestSubsystem();
	const FQuestId& ThisQuestId = QuestSubsystem.GetQuestIdFromFlow(GetFlowAsset());
	
	// for now assume unique tag per objective
	if (ThisQuestId == QuestId && CompletedObjectiveId == ObjectiveRef.ObjectiveId)
	{
		TriggerOutput(ObjectiveCompletedPinName, true);
	}
}

#if WITH_EDITOR
FString UQuestFlowNode_QuestObjective::GetNodeDescription() const
{
	return ObjectiveRef.ObjectiveId.ToString();
}

EDataValidationResult UQuestFlowNode_QuestObjective::ValidateNode()
{
	if (!ObjectiveRef.IsValid() || ObjectiveRef.QuestRef != GetOwningQuestAsset())
	{
		ValidationLog.Error<UFlowNode>(TEXT("ObjectiveRef is invalid or not referencing a quest objective from linked quest. Please reference a valid objective from graph linked quest."), this);
		return EDataValidationResult::Invalid;
	}

	return EDataValidationResult::Valid;
}

void UQuestFlowNode_QuestObjective::OnOwningQuestChanged()
{
	Super::OnOwningQuestChanged();

	TSoftObjectPtr<UQuestDataAsset> NewOwningQuestRef = GetOwningQuestAsset();

	if (ObjectiveRef.QuestRef != NewOwningQuestRef)
	{
		ObjectiveRef.QuestRef = NewOwningQuestRef;
		ObjectiveRef.FixupReference();
	}
}
#endif
