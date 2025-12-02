// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
	
	UPROPERTY(EditAnywhere)
	FName ObjectiveId;

#if WITH_EDITOR
	void FixupReference()
	{
		if (IsValid())
		{
			return;
		}

		if (QuestRef.ToSoftObjectPath().IsValid())
		{
			if (UQuestDataAsset* QuestAsset = QuestRef.LoadSynchronous())
			{
				if (QuestAsset->Objectives.Num() > 0)
				{
					ObjectiveId = QuestRef.LoadSynchronous()->Objectives[0]->ObjectiveId;
					return;
				}
			}
		}

		QuestRef = nullptr;
		ObjectiveId = NAME_None;
	}
	
	bool IsValid() const
	{
		if (!QuestRef.ToSoftObjectPath().IsValid() || !ObjectiveId.IsValid() || ObjectiveId == NAME_None)
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
