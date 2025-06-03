#include "DialogNode.h"

#include "DialogCondition.h"
#include "DialogTrigger.h"


bool UDialogNode::IsAvailable(UWorld* World) const
{
	bool bIsAvailable = true;

	for (const auto& Condition : Conditions)
	{
		bIsAvailable &= Condition->IsSatisfied(World);
	}

	return bIsAvailable;
}

void UDialogNode::Trigger(UWorld* World)
{
	for (const auto& Trigger : Triggers)
	{
		Trigger->Execute(World);
	}
}
