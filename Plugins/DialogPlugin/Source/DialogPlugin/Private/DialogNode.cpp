#include "DialogNode.h"

#include "DialogCondition.h"
#include "DialogTrigger.h"


bool UDialogNode::IsAvailable() const
{
	bool bIsAvailable = true;

	for (const auto& Condition : Conditions)
	{
		bIsAvailable &= Condition->IsSatisfied();
	}

	return bIsAvailable;
}

void UDialogNode::Trigger()
{
	for (const auto& Trigger : Triggers)
	{
		Trigger->Execute();
	}
}
