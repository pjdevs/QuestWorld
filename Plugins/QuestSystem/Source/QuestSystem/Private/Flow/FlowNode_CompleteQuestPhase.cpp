// Copyright pjdevs. All Rights Reserved.


#include "Flow/FlowNode_CompleteQuestPhase.h"
#include "QuestSubsystem.h"


UFlowNode_CompleteQuestPhase::UFlowNode_CompleteQuestPhase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
#endif
}

void UFlowNode_CompleteQuestPhase::ExecuteInput(const FName& PinName)
{
	UQuestSubsystem* QuestSubsystem = GetQuestSubsystem();
	const FQuestId QuestId = QuestSubsystem->GetQuestIdFromFlow(GetFlowAsset());

	QuestSubsystem->CompleteQuestActivePhase(QuestId);
	
	TriggerFirstOutput(true);
}

UQuestSubsystem* UFlowNode_CompleteQuestPhase::GetQuestSubsystem() const
{
	const UWorld* World = GetWorld();
	ensureMsgf(World != nullptr, TEXT("World is nullptr in FlowNode_QuestObjective"));
	
	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	ensureMsgf(World != nullptr, TEXT("QuestSubsystem is nullptr in FlowNode_QuestObjective"));

	return QuestSubsystem;
}

#if WITH_EDITOR
FString UFlowNode_CompleteQuestPhase::GetNodeDescription() const
{
	return FString("Active Phase");
}

EDataValidationResult UFlowNode_CompleteQuestPhase::ValidateNode()
{
	return EDataValidationResult::Valid;
}
#endif