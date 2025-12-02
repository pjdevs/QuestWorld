// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AddOns/FlowNodeAddOn.h"
#include "Assets/QuestObjectiveReference.h"
#include "Interfaces/FlowPredicateInterface.h"
#include "FlowNodeAddon_PredicateObjective.generated.h"

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
class QUESTSYSTEM_API UFlowNodeAddon_PredicateObjective : public UFlowNodeAddOn, public IFlowPredicateInterface
{
	GENERATED_BODY()

public:
	UFlowNodeAddon_PredicateObjective();

	virtual bool EvaluatePredicate_Implementation() const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Quest")
	EObjectiveCompletionBehavior Behavior;

	UPROPERTY(EditAnywhere, Category = "Quest", meta = (QuestReference = "GetOwningQuestAsset"))
	FQuestObjectiveReference ObjectiveRef;
};
