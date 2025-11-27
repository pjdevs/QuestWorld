// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestDataAsset.h"
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
	FName ObjectiveId;

#if WITH_EDITOR
	bool IsValid() const
	{
		if (QuestRef.IsNull() || !ObjectiveId.IsValid() || ObjectiveId == NAME_None)
		{
			return false;
		}

		UQuestDataAsset* QuestAsset = QuestRef.LoadSynchronous();

		if (QuestAsset == nullptr)
		{
			return false;
		}

		for (const TObjectPtr<UQuestObjective>& ObjectiveAsset : QuestAsset->Objectives)
		{
			if (ObjectiveId == ObjectiveAsset->ObjectiveId)
			{
				return true;
			}
		}

		return false;
	}
#endif
};
