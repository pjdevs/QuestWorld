// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/FlowNode.h"
#include "FlowNode_Quest.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Quest"))
class QUESTSYSTEM_API UFlowNode_Quest : public UFlowNode
{
	GENERATED_BODY()

public:
	UFlowNode_Quest(const FObjectInitializer& ObjectInitializer);

	virtual void ExecuteInput(const FName& PinName) override;

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;
	virtual EDataValidationResult ValidateNode() override;
#endif
	
protected:
	UPROPERTY(EditAnywhere, Category = "Quest", meta = (AllowedTypes = Quest))
	FPrimaryAssetId QuestId;
};
