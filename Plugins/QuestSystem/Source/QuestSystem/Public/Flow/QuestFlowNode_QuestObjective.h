// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestFlowNodeBase.h"
#include "QuestObjectiveCompletionState.h"
#include "Assets/QuestObjectiveReference.h"
#include "QuestFlowNode_QuestObjective.generated.h"

struct FQuestId;
class UQuestSubsystem;

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Quest Objective"))
class QUESTSYSTEM_API UQuestFlowNode_QuestObjective : public UQuestFlowNodeBase
{
	GENERATED_BODY()

public:
	UQuestFlowNode_QuestObjective(const FObjectInitializer& ObjectInitializer);

	virtual void ExecuteInput(const FName& PinName) override;
	virtual void Cleanup() override;
	virtual void OnLoad_Implementation() override;

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;
	virtual EDataValidationResult ValidateNode() override;
	virtual void OnOwningQuestChanged() override;
#endif

private:
	void OnObjectiveCompleted(
		const FQuestId& QuestId,
		const FName& CompletedObjectiveId,
		EQuestObjectiveCompletionState CompletionState
	);

protected:
	UPROPERTY(EditAnywhere, Category = "Quest", meta = (QuestReference = "GetOwningQuestAsset"))
	FQuestObjectiveReference ObjectiveRef;

private:
	FDelegateHandle ObjectiveCompletedDelegateHandle;
};
