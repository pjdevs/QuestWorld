// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GaspCharacterBase.h"
#include "GaspAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYABILITYSYSTEMPLUGIN_API AGaspAICharacter : public AGaspCharacterBase
{
	GENERATED_BODY()

public:
	AGaspAICharacter();

	virtual void BeginPlay() override;
};
