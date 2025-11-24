// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/Graph/FlowNode_Finish.h"
#include "FlowNode_CompleteQuest.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Complete Quest"))
class QUESTSYSTEM_API UFlowNode_CompleteQuest : public UFlowNode_Finish
{
	GENERATED_BODY()

public:
	UFlowNode_CompleteQuest(const FObjectInitializer& ObjectInitializer);
	
	virtual void ExecuteInput(const FName& PinName) override;
};
