// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveQuestObjective.h"
#include "ActiveQuest.generated.h"

class UQuestDataAsset;

/**
 * 
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FActiveQuest
{
	GENERATED_BODY()
	
	FActiveQuest() = default; // Needed for creating TArray etc. Will see if we use TUniquePtr or so later
	FActiveQuest(const FPrimaryAssetId& QuestId, UQuestDataAsset* QuestDataAsset, UWorld* World);
	~FActiveQuest();

	const FPrimaryAssetId& GetQuestId() const { return QuestId; }
	const TArray<FActiveQuestObjective>& GetObjectives() const { return Objectives; }
	bool IsCompleted() const { return bQuestCompleted; }
	void OnQuestEvent(UWorld* World, UBaseQuestEvent* Event);
	
private:
	UPROPERTY()
	FPrimaryAssetId QuestId;

	UPROPERTY()
	TObjectPtr<UQuestDataAsset> QuestDataAsset;
	
	UPROPERTY()
	TArray<FActiveQuestObjective> Objectives;

	bool bQuestCompleted;
};
