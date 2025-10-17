// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GaspCharacterBase.h"
#include "GaspPlayerCharacter.generated.h"

UCLASS()
class GAMEPLAYABILITYSYSTEMPLUGIN_API AGaspPlayerCharacter : public AGaspCharacterBase
{
	GENERATED_BODY()

public:
	AGaspPlayerCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	void InitAbilitySystemComponent();
};
