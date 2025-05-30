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

	UPROPERTY(EditDefaultsOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	FText Title;

	UPROPERTY(EditDefaultsOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UQuestObjective>> Objectives;

public:
	const FText& GetTitle() const { return Title; }
	const FText& GetDescription() const { return Description; }
	const TArray<TObjectPtr<UQuestObjective>>& GetQuestObjectives() const { return Objectives; }
	
	bool IsCompleted(UWorld* World) const;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
