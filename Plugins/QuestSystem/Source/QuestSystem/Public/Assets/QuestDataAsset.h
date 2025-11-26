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

#if WITH_EDITOR
public:
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

public:
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FGameplayTag QuestIdTag;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TArray<FGameplayTag> Phases;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TSet<FName> PhaseNames;
	
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
