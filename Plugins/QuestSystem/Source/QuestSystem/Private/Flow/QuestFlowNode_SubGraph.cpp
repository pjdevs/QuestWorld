// Copyright pjdevs. All Rights Reserved.


#include "Flow/QuestFlowNode_SubGraph.h"

#include "Flow/QuestFlowAsset.h"

UQuestFlowNode_SubGraph::UQuestFlowNode_SubGraph(const FObjectInitializer& ObjectInitializer)
	:  Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
	AllowedAssetClasses = { UQuestFlowAsset::StaticClass() };
#endif
}
