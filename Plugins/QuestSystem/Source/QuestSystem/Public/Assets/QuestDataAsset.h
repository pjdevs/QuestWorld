// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestObjective.h"
#include "Engine/DataAsset.h"
#include "QuestDataAsset.generated.h"

class UFlowAsset;
class UQuestObjective;

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FGameplayTag QuestIdTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FText Title;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Quest")
	TArray<TObjectPtr<UQuestObjective>> Objectives;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	bool bShouldAutocomplete;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	UFlowAsset* QuestFlowAsset;
};
