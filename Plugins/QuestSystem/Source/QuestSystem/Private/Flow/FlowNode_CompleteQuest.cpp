// Copyright pjdevs. All Rights Reserved.


#include "Flow/FlowNode_CompleteQuest.h"
#include "QuestSubsystem.h"


static const FName SucceedQuestPinName(TEXT("Succeed"));
static const FName FailQuestPinName(TEXT("Fail"));

UFlowNode_CompleteQuest::UFlowNode_CompleteQuest(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
#endif
	InputPins = { FFlowPin(SucceedQuestPinName), FFlowPin(FailQuestPinName) };
}

void UFlowNode_CompleteQuest::ExecuteInput(const FName& PinName)
{
	const UWorld* World = GetWorld();
	ensureMsgf(World != nullptr, TEXT("World is nullptr in UFlowNode_CompleteQuest"));
	
	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	ensureMsgf(World != nullptr, TEXT("QuestSubsystem is nullptr in UFlowNode_CompleteQuest"));

	const FQuestId QuestId = QuestSubsystem->GetQuestIdFromFlow(GetFlowAsset());

	if (PinName == SucceedQuestPinName)
	{
		QuestSubsystem->SucceedQuest(QuestId);
	}
	else if (PinName == FailQuestPinName)
	{
		QuestSubsystem->FailQuest(QuestId);
	}

	// Do not need to call finish as complete quest will finish the graph
}

