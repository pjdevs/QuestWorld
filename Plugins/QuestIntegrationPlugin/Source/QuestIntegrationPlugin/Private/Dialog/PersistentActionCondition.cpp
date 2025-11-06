// Copyright pjdevs. All Rights Reserved.


#include "Dialog/PersistentActionCondition.h"

#include "Actions/PersistentActionsStatics.h"


bool UPersistentActionCondition::IsSatisfied_Implementation(AController* DialogController)
{
	return bInvert
		? !UPersistentActionsStatics::HasActionBeenDone(GetWorld(), ActionToCheck)
		: UPersistentActionsStatics::HasActionBeenDone(GetWorld(), ActionToCheck);
}
