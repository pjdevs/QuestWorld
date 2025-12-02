// Copyright pjdevs. All Rights Reserved.


#include "Flow/QuestFlowNodeAddon_PredicateObjective.h"

#include "QuestSubsystem.h"
#include "Nodes/FlowNode.h"


bool UQuestFlowNodeAddon_PredicateObjective::EvaluatePredicate_Implementation() const
{
	const UQuestSubsystem& QuestSubsystem = GetQuestSubsystem(); 
	const FQuestId& QuestId = GetOwningQuestId();

	switch (Behavior)
	{
	case EObjectiveCompletionBehavior::Completed:
		return QuestSubsystem.IsObjectiveCompleted(QuestId, ObjectiveRef.ObjectiveId);
	case EObjectiveCompletionBehavior::Failed:
		return QuestSubsystem.IsObjectiveFailed(QuestId, ObjectiveRef.ObjectiveId);
	case EObjectiveCompletionBehavior::Succeeded:
		return QuestSubsystem.IsObjectiveSucceeded(QuestId, ObjectiveRef.ObjectiveId);
	default:
		return false;
	}
}

FString UQuestFlowNodeAddon_PredicateObjective::GetNodeDescription() const
{
	return ObjectiveRef.ObjectiveId.ToString();
}
