#include "DialogNode.h"

#include "DialogCondition.h"
#include "DialogTrigger.h"


bool UDialogNode::IsAvailable(UWorld* World, AController* DialogController) const
{
	bool bIsAvailable = true;

	for (const auto& Condition : Conditions)
	{
		bIsAvailable &= Condition->IsSatisfied(World, DialogController);
	}

	return bIsAvailable;
}

void UDialogNode::Trigger(UWorld* World, AController* DialogController)
{
	for (const auto& Trigger : Triggers)
	{
		Trigger->Execute(World, DialogController);
	}
}
