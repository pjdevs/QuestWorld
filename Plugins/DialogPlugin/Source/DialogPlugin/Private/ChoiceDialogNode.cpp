// Copyright pjdevs. All Rights Reserved.


#include "ChoiceDialogNode.h"

bool UChoiceDialogNode::IsAvailable() const
{
	bool bHasAvailableChoices = false;

	for (auto& ChildNode : GetNextDialogs())
	{
		bHasAvailableChoices |= ChildNode->IsAvailable();
	}

	return bHasAvailableChoices && Super::IsAvailable();
}
