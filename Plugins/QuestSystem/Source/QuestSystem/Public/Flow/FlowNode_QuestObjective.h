// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestObjectiveCompletionState.h"
#include "Assets/QuestObjectiveReference.h"
#include "Nodes/FlowNode.h"
#include "FlowNode_QuestObjective.generated.h"

struct FQuestId;
class UQuestSubsystem;

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
	virtual void Cleanup() override;
	virtual void OnLoad_Implementation() override;

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;
	virtual EDataValidationResult ValidateNode() override;
#endif

private:
	UQuestSubsystem* GetQuestSubsystem() const;
	void OnObjectiveCompleted(
		const FQuestId& QuestId,
		const FName& CompletedObjectiveId,
		EQuestObjectiveCompletionState CompletionState
	);

protected:
	UPROPERTY(EditAnywhere, Category = "Quest")
	FQuestObjectiveReference ObjectiveRef;

private:
	FDelegateHandle ObjectiveCompletedDelegateHandle;
};
