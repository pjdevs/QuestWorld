// Copyright pjdevs. All Rights Reserved.


#include "Dialog/PersistentActionCondition.h"

#include "Actions/PersistentActionsStatics.h"

bool UPersistentActionCondition::IsSatisfied(UWorld* World)
{
	return bInvert
		? !UPersistentActionsStatics::HasActionBeenDone(World, ActionToCheck)
		: UPersistentActionsStatics::HasActionBeenDone(World, ActionToCheck);
}
