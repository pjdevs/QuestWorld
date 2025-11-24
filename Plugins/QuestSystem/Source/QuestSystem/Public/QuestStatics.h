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
	UFUNCTION(BlueprintPure, Category = Quest, meta=(WorldContext="WorldContextObject"))
	static UQuestComponent* GetQuestComponent(UObject* WorldContextObject);
};
