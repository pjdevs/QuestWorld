// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestFlowNodeBase.h"
#include "Assets/QuestPhaseReference.h"
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
	virtual void OnOwningQuestChanged() override;
#endif

protected:
	UPROPERTY(EditAnywhere, Category = "Quest", meta = (QuestReference = "GetOwningQuestAsset"))
	FQuestPhaseReference PhaseRef;
};
