// Copyright pjdevs. All Rights Reserved.


#include "ChoiceDialogNode.h"

bool UChoiceDialogNode::IsAvailable(UWorld* World) const
{
	bool bHasAvailableChoices = false;

	for (auto& ChildNode : GetNextDialogs())
	{
		bHasAvailableChoices |= ChildNode->IsAvailable(World);
	}

	return bHasAvailableChoices && Super::IsAvailable(World);
}
