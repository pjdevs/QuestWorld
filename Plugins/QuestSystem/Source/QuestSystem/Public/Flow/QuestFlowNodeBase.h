// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestId.h"
#include "Nodes/FlowNode.h"
#include "QuestFlowNodeBase.generated.h"

class UQuestSubsystem;

/**
 * Base class for nodes of quest flow graph.
 */
UCLASS(Abstract, BlueprintType, HideCategories = Object)
class QUESTSYSTEM_API UQuestFlowNodeBase : public UFlowNode
{
	GENERATED_BODY()

public:
	UQuestFlowNodeBase(const FObjectInitializer& ObjectInitializer);
	
protected:
	UQuestSubsystem& GetQuestSubsystem() const;
	FQuestId GetOwningQuestId() const;

#if WITH_EDITOR
public:
	virtual void OnOwningQuestChanged();
	
protected:
	UFUNCTION()
	TSoftObjectPtr<UQuestDataAsset> GetOwningQuestAsset() const;
#endif
};
