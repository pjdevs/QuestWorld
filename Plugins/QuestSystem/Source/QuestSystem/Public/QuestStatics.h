// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuestStatics.generated.h"

class UBaseQuestEvent;
struct FQuestDescription;

class UQuestComponent;
/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Quest, meta=(WorldContext="WorldContextObject"))
	static void StartQuest(UObject* WorldContextObject, FPrimaryAssetId QuestId);

	UFUNCTION(BlueprintPure, Category = Quest, meta=(WorldContext="WorldContextObject"))
	static TArray<FQuestDescription> GetActiveQuests(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = Quest, meta=(WorldContext="WorldContextObject"))
	static TArray<FQuestDescription> GetCompletedQuests(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = Quest, meta=(WorldContext="WorldContextObject"))
	static bool IsQuestCompleted(UObject* WorldContextObject, FPrimaryAssetId QuestId);

	UFUNCTION(BlueprintPure, Category = Quest, meta=(WorldContext="WorldContextObject"))
	static bool IsQuestActive(UObject* WorldContextObject, FPrimaryAssetId QuestId);
	
	UFUNCTION(BlueprintCallable, Category = Quest, meta=(WorldContext="WorldContextObject"))
	static void SubmitQuestEvent(UObject* WorldContextObject, UBaseQuestEvent* Event);

	UFUNCTION(BlueprintPure, Category = Quest, meta=(WorldContext="WorldContextObject"))
	static UQuestComponent* GetQuestComponent(UObject* WorldContextObject);
};
