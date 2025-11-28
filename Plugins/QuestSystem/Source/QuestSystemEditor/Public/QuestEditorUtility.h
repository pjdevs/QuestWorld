// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QuestEditorUtility.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEMEDITOR_API UQuestEditorUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Quest")
	static UClass* PickObjectiveClass();
};
