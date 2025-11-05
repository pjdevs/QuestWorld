// Copyright pjdevs. All Rights Reserved.


#include "DemoPlayerState.h"

void ADemoPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (ADemoPlayerState* NewPlayerState = Cast<ADemoPlayerState>(PlayerState))
	{
		NewPlayerState->bIsInit = bIsInit;
		NewPlayerState->PlayerIndex = PlayerIndex;
	}
}
