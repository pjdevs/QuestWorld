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
	
	FActiveQuest(const FPrimaryAssetId& QuestId, UQuestDataAsset* QuestDataAsset, UWorld* World);
	~FActiveQuest();

	const FPrimaryAssetId& GetQuestId() const { return QuestId; }
	const FActiveQuestObjective& GetCurrentQuestObjective() const { return Objectives[CurrentObjectiveIndex]; }
	bool IsCompleted() const { return bQuestCompleted; }

	void OnQuestEvent(const FName& EventType, AActor* EventActor, UWorld* World);
	
private:
	UPROPERTY()
	FPrimaryAssetId QuestId;

	UPROPERTY()
	TObjectPtr<UQuestDataAsset> QuestDataAsset;
	
	UPROPERTY()
	TArray<FActiveQuestObjective> Objectives;

	int32 CurrentObjectiveIndex;
	bool bQuestCompleted;
};
