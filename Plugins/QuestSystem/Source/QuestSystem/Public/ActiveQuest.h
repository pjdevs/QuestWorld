// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveQuestObjective.h"
#include "ActiveQuest.generated.h"

class UQuestDataAsset;

/**
 * Runtime representation of a currently active / started / in-progress quest.
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FActiveQuest
{
	GENERATED_BODY()

public:
	FActiveQuest() = default; // Needed for creating TArray etc. Will see if we use TUniquePtr or so later
	FActiveQuest(const FPrimaryAssetId& QuestId, UQuestDataAsset* QuestDataAsset, UWorld* World);

	FActiveQuestObjective& GetObjective(int Index) { return Objectives[Index]; }
	const TArray<FActiveQuestObjective>& GetObjectives() const { return Objectives; }
	const FPrimaryAssetId& GetQuestId() const { return QuestId; }
	bool IsCompleted() const { return bQuestCompleted; }
	int GetCurrentObjectiveIndex() const { return CurrentObjectiveIndex; }

	void RestoreCurrentObjectiveIndex(int ObjectiveIndex);
	
	/**
	 * Notify the quest of an emitted quest event.
	 * @param World 
	 * @param Event 
	 * @return true if the event made some progression or false else.
	 */
	bool OnQuestEvent(UWorld* World, UBaseQuestEvent* Event);
	
private:
	UPROPERTY()
	TObjectPtr<UQuestDataAsset> QuestDataAsset;
	
	UPROPERTY()
	TArray<FActiveQuestObjective> Objectives;

	FPrimaryAssetId QuestId;
	bool bQuestCompleted;
	bool bIsSequential;
	int CurrentObjectiveIndex;
};
