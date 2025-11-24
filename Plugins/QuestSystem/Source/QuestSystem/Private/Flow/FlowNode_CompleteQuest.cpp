// Copyright pjdevs. All Rights Reserved.


#include "Flow/FlowNode_CompleteQuest.h"
#include "QuestSubsystem.h"

UFlowNode_CompleteQuest::UFlowNode_CompleteQuest(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
#endif
}

void UFlowNode_CompleteQuest::ExecuteInput(const FName& PinName)
{
	const UWorld* World = GetWorld();
	ensureMsgf(World != nullptr, TEXT("World is nullptr in FlowNode_Quest"));
	
	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	ensureMsgf(World != nullptr, TEXT("QuestSubsystem is nullptr in FlowNode_Quest"));

	const FQuestId QuestId = QuestSubsystem->GetQuestIdFromFlow(GetFlowAsset());
	QuestSubsystem->CompleteQuest(QuestId);
}

