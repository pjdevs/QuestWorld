// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "QuestObjective.generated.h"

class UBaseQuestEvent;

/**
 * Objective representation to use in quest data assets.
 * Can be subclassed to use counter functionality or use this one for manual completion.
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class QUESTSYSTEM_API UQuestObjective : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * The current objective progression to TargetValue. Only for retro completable objectives.
	 */
	UFUNCTION(BlueprintNativeEvent)
	int GetCompletion(UWorld* World) const;

	/**
	 * The target progression value to complete this objective.
	 */
	UFUNCTION(BlueprintNativeEvent)
	int GetTargetValue() const;
	
	/**
	 * Try progress this objective with the given quest event.
	 * Returns how much progress has been made or 0 if none.
	 * Will use the return value as new progress if bShouldAddProgress instead of adding.
	 */
	UFUNCTION(BlueprintNativeEvent)
	int TriggerProgress(UWorld* World, UBaseQuestEvent* Event) const;
	
protected:
	virtual int GetCompletion_Implementation(UWorld* World) const { return 0; }

	virtual int GetTargetValue_Implementation() const { return 0; }
	
	virtual int TriggerProgress_Implementation(UWorld* World, UBaseQuestEvent* Event) const { return 0; }

public:
	/**
	 * Id of the objective.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Objective")
	FGameplayTag ObjectiveId;
	
	/**
	 * Can the objective be completed before this quest objective start and hence be checked right after it started.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Objective")
	FText ObjectiveDescription;

	/**
	 * Can the objective be completed before this quest objective start and hence be checked right after it started.
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Objective")
	bool bIsRetroCompletable = false;

	/**
	 * When triggered, should we add the return value of the trigger function to progress
	 * or use it as new the progress value. 
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Objective")
	bool bShouldAddProgress = true;
};
