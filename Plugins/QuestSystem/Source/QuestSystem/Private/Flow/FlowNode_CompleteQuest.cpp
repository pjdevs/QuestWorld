// Copyright pjdevs. All Rights Reserved.


#include "Flow/FlowNode_CompleteQuest.h"
#include "QuestSubsystem.h"


static const FName SucceedPinName(TEXT("Succeed"));
static const FName FailPinName(TEXT("Fail"));

UFlowNode_CompleteQuest::UFlowNode_CompleteQuest(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
#endif
	InputPins = {  };
}

void UFlowNode_CompleteQuest::ExecuteInput(const FName& PinName)
{
	const UWorld* World = GetWorld();
	ensureMsgf(World != nullptr, TEXT("World is nullptr in FlowNode_Quest"));
	
	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	ensureMsgf(World != nullptr, TEXT("QuestSubsystem is nullptr in FlowNode_Quest"));

	const FQuestId QuestId = QuestSubsystem->GetQuestIdFromFlow(GetFlowAsset());

	if (PinName == SucceedPinName)
	{
		QuestSubsystem->SucceedQuest(QuestId);
	}
	else if (PinName == FailPinName)
	{
		QuestSubsystem->FailQuest(QuestId);
	}

	// Do not need to call finish as complete quest will finish the graph
}

