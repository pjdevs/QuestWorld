// Copyright pjdevs. All Rights Reserved.


#include "IPStatefulActor.h"
#include "IPStatefulComponent.h"

AIPStatefulActor::AIPStatefulActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = true;

	StatefulComponent = CreateDefaultSubobject<UIPStatefulComponent>("Stateful Component");
}
