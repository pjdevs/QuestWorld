// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "QuestRef.IsValid()"))
	FName ObjectiveIdName;

public:
	FGameplayTag GetObjectiveId() const { return FGameplayTag::RequestGameplayTag(ObjectiveIdName); }

#if WITH_EDITOR
	bool IsValid() const
	{
		return !QuestRef.IsNull() && FGameplayTag::RequestGameplayTag(ObjectiveIdName, false).IsValid();
	}
#endif
};
