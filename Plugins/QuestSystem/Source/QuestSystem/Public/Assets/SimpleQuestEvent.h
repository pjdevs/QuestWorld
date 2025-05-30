// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseQuestEvent.h"
#include "UObject/Object.h"
#include "SimpleQuestEvent.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API USimpleQuestEvent : public UBaseQuestEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	FName TagAdded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest)
	APlayerController* AffectedPlayerController;
};
