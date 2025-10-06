// Copyright pjdevs. All Rights Reserved.


#include "ChoiceDialogNode.h"
#include "DialogChoice.h"


bool UChoiceDialogNode::IsAvailable(UWorld* World) const
{
	bool bHasAvailableChoices = false;

	for (const TObjectPtr<UDialogChoice>& Choice : DialogChoices)
	{
		if (const UDialogNode* NextDialog = Choice->GetNextDialog())
		{
			bHasAvailableChoices |= NextDialog->IsAvailable(World);
		}
	}

	return bHasAvailableChoices && Super::IsAvailable(World);
}
