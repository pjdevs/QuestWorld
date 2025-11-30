// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/FlowNode.h"
#include "FlowNode_CompleteQuestPhase.generated.h"

class UQuestPhaseReference;
struct FQuestId;
class UQuestSubsystem;

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Complete Quest Phase"))
class QUESTSYSTEM_API UFlowNode_CompleteQuestPhase : public UFlowNode
{
	GENERATED_BODY()

public:
	UFlowNode_CompleteQuestPhase(const FObjectInitializer& ObjectInitializer);

	virtual void ExecuteInput(const FName& PinName) override;

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;
	virtual EDataValidationResult ValidateNode() override;
#endif

protected:
	UQuestSubsystem* GetQuestSubsystem() const;
};
