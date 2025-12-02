// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/Graph/FlowNode_SubGraph.h"
#include "QuestFlowNode_SubGraph.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Quest Sub Graph"))
class QUESTSYSTEM_API UQuestFlowNode_SubGraph : public UFlowNode_SubGraph
{
	GENERATED_BODY()

public:
	UQuestFlowNode_SubGraph(const FObjectInitializer& ObjectInitializer);
};
