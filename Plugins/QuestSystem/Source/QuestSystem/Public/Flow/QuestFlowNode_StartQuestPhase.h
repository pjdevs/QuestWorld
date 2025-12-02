// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestFlowNodeBase.h"
#include "QuestFlowNode_StartQuestPhase.generated.h"

class UQuestPhaseReference;
struct FQuestId;
class UQuestSubsystem;

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Start Quest Phase"))
class QUESTSYSTEM_API UQuestFlowNode_StartQuestPhase : public UQuestFlowNodeBase
{
	GENERATED_BODY()

public:
	UQuestFlowNode_StartQuestPhase(const FObjectInitializer& ObjectInitializer);

	virtual void ExecuteInput(const FName& PinName) override;

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;
	virtual EDataValidationResult ValidateNode() override;
#endif

protected:
	UPROPERTY(EditAnywhere, Instanced, Category = "Quest", meta = (AllowNone = false))
	TObjectPtr<UQuestPhaseReference> PhaseRef;
};
