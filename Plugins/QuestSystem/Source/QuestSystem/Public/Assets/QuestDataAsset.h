// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestDataAsset.generated.h"

class UQuestStep;
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
	TArray<TObjectPtr<UQuestStep>> Steps;

public:
	const FText& GetTitle() const { return Title; }
	const FText& GetDescription() const { return Description; }
	const TArray<TObjectPtr<UQuestStep>>& GetQuestSteps() const { return Steps; }
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
