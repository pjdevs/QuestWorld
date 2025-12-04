// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestObjectiveReference.generated.h"


class UQuestDataAsset;

/**
 * 
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FQuestObjectiveReference
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UQuestDataAsset> QuestRef;
#endif
	
	UPROPERTY(EditAnywhere)
	FName ObjectiveId;

#if WITH_EDITOR
	void FixupReference();
	bool IsValid() const;
#endif
};
