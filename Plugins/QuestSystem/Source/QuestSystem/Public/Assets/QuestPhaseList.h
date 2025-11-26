// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "QuestPhaseList.generated.h"

class UQuestDataAsset;

/**
 * 
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FQuestPhaseList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer Phases;

#if WITH_EDITOR
public:
	void OnOwningQuestChanged(const UQuestDataAsset* OwningQuestAsset);
#endif
};
