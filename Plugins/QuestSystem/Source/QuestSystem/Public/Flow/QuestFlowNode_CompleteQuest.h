// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestFlowNodeBase.h"
#include "QuestFlowNode_CompleteQuest.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Complete Quest"))
class QUESTSYSTEM_API UQuestFlowNode_CompleteQuest : public UQuestFlowNodeBase
{
	GENERATED_BODY()

public:
	UQuestFlowNode_CompleteQuest(const FObjectInitializer& ObjectInitializer);
	
	virtual void ExecuteInput(const FName& PinName) override;
	virtual bool CanFinishGraph() const override;
};
