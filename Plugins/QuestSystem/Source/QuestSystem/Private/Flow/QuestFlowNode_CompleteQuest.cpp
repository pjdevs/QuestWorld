// Copyright pjdevs. All Rights Reserved.


#include "Flow/QuestFlowNode_CompleteQuest.h"
#include "QuestSubsystem.h"


static const FName SucceedQuestPinName(TEXT("Succeed"));
static const FName FailQuestPinName(TEXT("Fail"));

UQuestFlowNode_CompleteQuest::UQuestFlowNode_CompleteQuest(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	NodeDisplayStyle = FlowNodeStyle::Terminal;
	Category = TEXT("Quest");
#endif
	InputPins = { FFlowPin(SucceedQuestPinName), FFlowPin(FailQuestPinName) };
}

void UQuestFlowNode_CompleteQuest::ExecuteInput(const FName& PinName)
{
	const FQuestId& QuestId = GetOwningQuestId();
	UQuestSubsystem& QuestSubsystem = GetQuestSubsystem();

	if (PinName == SucceedQuestPinName)
	{
		QuestSubsystem.SucceedQuest(QuestId);
	}
	else if (PinName == FailQuestPinName)
	{
		QuestSubsystem.FailQuest(QuestId);
	}

	// Do not need to call finish as complete quest will finish the graph
}

bool UQuestFlowNode_CompleteQuest::CanFinishGraph() const
{
	return true;
}

