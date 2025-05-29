// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestObjective.h"
#include "Engine/DataAsset.h"
#include "QuestDataAsset.generated.h"

class UQuestObjective;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UQuestObjective>> Objectives;

public:
	const TArray<TObjectPtr<UQuestObjective>>& GetQuestObjectives() const { return Objectives; }
	
	bool IsCompleted(UWorld* World) const;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
