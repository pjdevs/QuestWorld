// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GaspAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYABILITYSYSTEMPLUGIN_API UGaspAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UGaspAbilitySystemComponent();

protected:
	virtual void BeginPlay() override;
};
