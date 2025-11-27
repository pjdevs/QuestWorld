// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestPhaseReferences.generated.h"

class UQuestDataAsset;

/**
 * 
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FQuestPhaseReferences
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSet<FName> Phases;

#if WITH_EDITOR
public:
	void OnOwningQuestChanged(const UQuestDataAsset* OwningQuestAsset);
#endif
};
