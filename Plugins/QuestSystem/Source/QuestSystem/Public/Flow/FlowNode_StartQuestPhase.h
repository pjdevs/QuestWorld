// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/FlowNode.h"
#include "FlowNode_StartQuestPhase.generated.h"

class UQuestPhaseReference;
struct FQuestId;
class UQuestSubsystem;

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Start Quest Phase"))
class QUESTSYSTEM_API UFlowNode_StartQuestPhase : public UFlowNode
{
	GENERATED_BODY()

public:
	UFlowNode_StartQuestPhase(const FObjectInitializer& ObjectInitializer);

	virtual void ExecuteInput(const FName& PinName) override;

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;
	virtual EDataValidationResult ValidateNode() override;
#endif

protected:
	UQuestSubsystem* GetQuestSubsystem() const;

protected:
	UPROPERTY(EditAnywhere, Instanced, Category = "Quest", meta = (AllowNone = false))
	TObjectPtr<UQuestPhaseReference> PhaseRef;
};
