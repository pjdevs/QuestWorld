// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "QuestSave.generated.h"

struct FQuestDescription;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class QUESTSYSTEM_API UQuestSave : public USaveGame
{
	GENERATED_BODY()

public:
	// TODO Save lighter and specific object than FQuestDescription

	UPROPERTY()
	TArray<FQuestDescription> ActiveQuests;

	UPROPERTY()
	TArray<FPrimaryAssetId> CompletedQuests;
};
