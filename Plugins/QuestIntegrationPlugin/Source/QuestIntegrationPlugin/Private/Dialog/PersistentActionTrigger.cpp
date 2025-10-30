// Copyright pjdevs. All Rights Reserved.


#include "Dialog/PersistentActionTrigger.h"
#include "Actions/PersistentActionsStatics.h"

void UPersistentActionTrigger::OnExecute_Implementation(AController* DialogController)
{
	UPersistentActionsStatics::SendPersistentAction(GetWorld(), ActionToTrigger, DialogController);
}
