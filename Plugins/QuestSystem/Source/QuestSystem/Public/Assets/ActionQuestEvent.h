// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseQuestEvent.h"
#include "UObject/Object.h"
#include "ActionQuestEvent.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UActionQuestEvent : public UBaseQuestEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest, meta = (ExposeOnSpawn = "true"))
	FName ActionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest, meta = (ExposeOnSpawn = "true"))
	AActor* ActionInstigator;
};
