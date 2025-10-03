// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Assets/BaseQuestEvent.h"
#include "InventoryQuestEvent.generated.h"


/**
 * 
 */
UCLASS()
class QUESTWORLD_API UInventoryQuestEvent : public UBaseQuestEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest, meta = (ExposeOnSpawn = "true"))
	FString NameOfItemAdded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest, meta = (ExposeOnSpawn = "true"))
	int CountOfItemAdded;
};
