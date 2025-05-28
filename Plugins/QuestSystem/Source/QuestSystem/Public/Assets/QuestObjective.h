// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestObjective.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class QUESTSYSTEM_API UQuestObjective : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Quest")
	bool IsSatisfied(UWorld* World, APlayerController* Player);

protected:
	virtual bool IsSatisfied_Implementation(UWorld* World, APlayerController* Player)
	PURE_VIRTUAL(UQuestObjective::IsSatisfied_Implementation, return false;);
};
