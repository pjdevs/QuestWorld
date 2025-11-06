// Copyright pjdevs. All Rights Reserved.


#include "ChoiceDialogNode.h"
#include "DialogChoice.h"


bool UChoiceDialogNode::IsAvailable(UWorld* World, AController* DialogController) const
{
	bool bHasAvailableChoices = false;

	for (const TObjectPtr<UDialogChoice>& Choice : DialogChoices)
	{
		if (const UDialogNode* NextDialog = Choice->GetNextDialog())
		{
			bHasAvailableChoices |= NextDialog->IsAvailable(World, DialogController);
		}
		else
		{
			bHasAvailableChoices = true; // no child so dialog is available, it will just end conversation
		}
	}

	return bHasAvailableChoices && Super::IsAvailable(World, DialogController);
}
