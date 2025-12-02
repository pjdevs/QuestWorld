// Copyright pjdevs. All Rights Reserved.


#include "Flow/QuestFlowNode_CompleteQuestPhase.h"
#include "QuestSubsystem.h"


UQuestFlowNode_CompleteQuestPhase::UQuestFlowNode_CompleteQuestPhase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
#endif
}

void UQuestFlowNode_CompleteQuestPhase::ExecuteInput(const FName& PinName)
{
	const FQuestId& QuestId = GetOwningQuestId();
	UQuestSubsystem& QuestSubsystem = GetQuestSubsystem();

	QuestSubsystem.CompleteQuestActivePhase(QuestId);
	
	TriggerFirstOutput(true);
}

#if WITH_EDITOR
FString UQuestFlowNode_CompleteQuestPhase::GetNodeDescription() const
{
	return FString("Active Phase");
}

EDataValidationResult UQuestFlowNode_CompleteQuestPhase::ValidateNode()
{
	return EDataValidationResult::Valid;
}
#endif