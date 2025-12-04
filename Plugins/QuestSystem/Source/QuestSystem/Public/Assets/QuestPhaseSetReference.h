// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestPhaseSetReference.generated.h"

class UQuestDataAsset;

/**
 * 
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FQuestPhaseSetReference
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FName> Phases;

public:
	FString ToString() const;

#if WITH_EDITOR
public:
	void OnOwningQuestChanged(const UQuestDataAsset* OwningQuestAsset);
#endif
};
