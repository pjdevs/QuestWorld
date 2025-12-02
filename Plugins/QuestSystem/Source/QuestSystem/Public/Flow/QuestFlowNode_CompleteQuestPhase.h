// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestFlowNodeBase.h"
#include "QuestFlowNode_CompleteQuestPhase.generated.h"

class UQuestPhaseReference;
struct FQuestId;
class UQuestSubsystem;

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Complete Quest Phase"))
class QUESTSYSTEM_API UQuestFlowNode_CompleteQuestPhase : public UQuestFlowNodeBase
{
	GENERATED_BODY()

public:
	UQuestFlowNode_CompleteQuestPhase(const FObjectInitializer& ObjectInitializer);

	virtual void ExecuteInput(const FName& PinName) override;

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;
	virtual EDataValidationResult ValidateNode() override;
#endif
};
