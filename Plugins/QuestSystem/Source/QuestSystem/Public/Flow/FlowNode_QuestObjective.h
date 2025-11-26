// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Assets/QuestPhaseReference.h"
#include "Nodes/FlowNode.h"
#include "FlowNode_QuestObjective.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Quest Objective"))
class QUESTSYSTEM_API UFlowNode_QuestObjective : public UFlowNode
{
	GENERATED_BODY()

public:
	UFlowNode_QuestObjective(const FObjectInitializer& ObjectInitializer);
	
	virtual void ExecuteInput(const FName& PinName) override;

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;
	virtual EDataValidationResult ValidateNode() override;
#endif

protected:
	UPROPERTY(EditAnywhere, Category = "Quest")
	FGameplayTag ObjectiveId;

	UPROPERTY(EditAnywhere, Instanced, Category = "Quest")
	UQuestPhaseReference* Phase;
};
