// Copyright pjdevs. All Rights Reserved.


#include "Flow/QuestFlowStatics.h"

#include "Flow/QuestFlowAsset.h"
#include "Nodes/FlowNodeBase.h"
#include "QuestSubsystem.h"


UQuestSubsystem& FQuestFlowStatics::GetQuestSubsystemFromFlowNodeBase(const UFlowNodeBase* FlowNodeBase)
{
	const FString& ThisClassName = FlowNodeBase->GetClass()->GetFName().ToString();
	
	const UWorld* World = FlowNodeBase->GetWorld();
	ensureMsgf(World != nullptr, TEXT("World is nullptr in node %s."), *ThisClassName);
	
	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	ensureMsgf(QuestSubsystem != nullptr, TEXT("QuestSubsystem is nullptr in node %s."), *ThisClassName);

	return *QuestSubsystem;
}

TSoftObjectPtr<UQuestDataAsset> FQuestFlowStatics::GetOwningQuestAssetFromFlowNodeBase(const UFlowNodeBase* FlowNodeBase)
{
	const UQuestFlowAsset* OwningQuestFlowAsset = Cast<UQuestFlowAsset>(
		FlowNodeBase->GetFlowAsset()
	);
	ensureMsgf(
		OwningQuestFlowAsset != nullptr,
		TEXT("%s should be owned by a %s."),
		*FlowNodeBase->GetClass()->GetFName().ToString(),
		*UQuestFlowAsset::StaticClass()->GetFName().ToString()
	);

	return OwningQuestFlowAsset->LinkedQuestRef;
}
