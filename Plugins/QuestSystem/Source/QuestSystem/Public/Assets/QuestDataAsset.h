// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "QuestObjective.h"
#include "Engine/DataAsset.h"
#include "QuestDataAsset.generated.h"

class UQuestFlowAsset;
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

#if WITH_EDITOR
public:
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	FGameplayTag QuestType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	FGameplayTagContainer QuestTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	TSet<FName> Phases;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	FText Title;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	FText Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced, Category = "Quest")
	TArray<TObjectPtr<UQuestObjective>> Objectives;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	TSoftObjectPtr<UQuestFlowAsset> QuestFlowAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Quest")
	bool bShouldAutocomplete;
};
