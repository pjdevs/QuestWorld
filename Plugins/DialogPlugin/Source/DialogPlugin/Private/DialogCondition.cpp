// Copyright pjdevs. All Rights Reserved.


#include "DialogCondition.h"

UWorld* UDialogCondition::GetWorld() const
{
	return World;
}

bool UDialogCondition::IsSatisfied(UWorld* InWorld, AController* DialogController)
{
	World = InWorld;

	const bool bIsSatisfied = IsSatisfied(DialogController);
	
	World = nullptr;

	return bIsSatisfied;
}

bool UDialogCondition::IsSatisfied_Implementation(AController* DialogController)
{
	return true;
}
