// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestId.h"
#include "Nodes/FlowNode.h"
#include "FlowNode_QuestObjective.generated.h"

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

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;
	virtual EDataValidationResult ValidateNode() override;
#endif

private:
	UQuestSubsystem* GetQuestSubsystem() const;
	void OnObjectiveCompleted(const FQuestId& QuestId, const FGameplayTag& CompletedObjectiveId);

protected:
	UPROPERTY(EditAnywhere, Category = "Quest")
	FGameplayTag ObjectiveId;

private:
	FDelegateHandle ObjectiveCompletedDelegateHandle;
};
