// Copyright pjdevs. All Rights Reserved.


#include "ChoiceDialogNode.h"

bool UChoiceDialogNode::IsAvailable() const
{
	bool bHasAvailableChoices = true;

	for (auto& ChildNode : GetNextDialogs())
	{
		bHasAvailableChoices &= ChildNode->IsAvailable();
	}

	return bHasAvailableChoices && Super::IsAvailable();
}
