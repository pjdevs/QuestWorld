// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestFlowNodeAddonBase.h"
#include "AddOns/FlowNodeAddOn.h"
#include "Assets/QuestObjectiveReference.h"
#include "Interfaces/FlowPredicateInterface.h"
#include "QuestFlowNodeAddon_PredicateObjective.generated.h"

UENUM()
enum class EObjectiveCompletionBehavior : uint8
{
	Completed,
	Failed,
	Succeeded
};

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "Is Objective Completed"))
class QUESTSYSTEM_API UQuestFlowNodeAddon_PredicateObjective : public UQuestFlowNodeAddonBase, public IFlowPredicateInterface
{
	GENERATED_BODY()

public:
	virtual bool EvaluatePredicate_Implementation() const override;

#if WITH_EDITOR
public:
	virtual FString GetNodeDescription() const override;
#endif

protected:
	UPROPERTY(EditAnywhere, Category = "Quest")
	EObjectiveCompletionBehavior Behavior;

	UPROPERTY(EditAnywhere, Category = "Quest", meta = (QuestReference = "GetOwningQuestAsset"))
	FQuestObjectiveReference ObjectiveRef;
};
