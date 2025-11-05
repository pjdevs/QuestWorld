// Copyright pjdevs. All Rights Reserved.


#include "DemoPlayerState.h"


void ADemoPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (ADemoPlayerState* NewDemoPlayerState = Cast<ADemoPlayerState>(PlayerState))
	{
		NewDemoPlayerState->bIsInit = bIsInit;
		NewDemoPlayerState->PlayerIndex = PlayerIndex;
	}
}
