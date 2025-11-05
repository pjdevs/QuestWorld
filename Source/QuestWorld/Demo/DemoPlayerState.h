// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GaspPlayerState.h"
#include "DemoPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class QUESTWORLD_API ADemoPlayerState : public AGaspPlayerState
{
	GENERATED_BODY()

protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;

public:
	int PlayerIndex = 0;
	bool bIsInit = false;
};
