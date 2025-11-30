// Copyright pjdevs. All Rights Reserved.


#include "Flow/FlowNodeAddon_PredicateObjective.h"

#include "QuestSubsystem.h"
#include "Nodes/FlowNode.h"


UFlowNodeAddon_PredicateObjective::UFlowNodeAddon_PredicateObjective()
	: Super()
{
#if WITH_EDITOR
	NodeDisplayStyle = FlowNodeStyle::AddOn_Predicate;
	Category = TEXT("Quest");
#endif
}

bool UFlowNodeAddon_PredicateObjective::EvaluatePredicate_Implementation() const
{
	const UWorld* World = GetWorld();
	ensureMsgf(World != nullptr, TEXT("World is nullptr in UFlowNodeAddon_PredicateObjective"));

	const UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	ensureMsgf(World != nullptr, TEXT("QuestSubsystem is nullptr in UFlowNodeAddon_PredicateObjective"));

	const FQuestId& QuestId = QuestSubsystem->GetQuestIdFromFlow(GetFlowNode()->GetFlowAsset());

	switch (Behavior)
	{
	case EObjectiveCompletionBehavior::Completed:
		return QuestSubsystem->IsObjectiveCompleted(QuestId, ObjectiveRef.ObjectiveId);
	case EObjectiveCompletionBehavior::Failed:
		return QuestSubsystem->IsObjectiveFailed(QuestId, ObjectiveRef.ObjectiveId);
	case EObjectiveCompletionBehavior::Succeeded:
		return QuestSubsystem->IsObjectiveSucceeded(QuestId, ObjectiveRef.ObjectiveId);
	default:
		return false;
	}
}
