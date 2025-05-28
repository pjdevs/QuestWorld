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
	FName QuestId;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	TArray<UQuestObjective*> Objectives;

public:
	const FName& GetQuestId() const { return QuestId; }
	bool IsCompleted(UWorld* World, APlayerController* PlayerController) const;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
