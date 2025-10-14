// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestObjective.generated.h"

class UBaseQuestEvent;
/**
 * Objective representation to use in quest data assets.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class QUESTSYSTEM_API UQuestObjective : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Is the objective retro completable
	 * (i.e. should check at objective creation if condition could be already be fulfilled).
	 */
	bool IsRetroCompletable() const { return bRetroCompletable; }

	/**
	 * Is the objective retro completable.
	 */
	bool ShouldAddProgress() const { return bShouldAddProgress; }
	
	/**
	 * The description of this objective to display.
	 */
	const FText& GetObjectiveDescription() const { return ObjectiveDescription; }
	
	/**
	 * The current objective progression to TargetValue. Only for retro completable events.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Quest)
	int GetCompletion(UWorld* World);

	/**
	 * The target progression value to complete this objective.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Quest)
	int GetTargetValue();
	
	/**
	 * Should the objective trigger for this event and payload.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Quest)
	int TriggerProgress(UWorld* World, UBaseQuestEvent* Event);
	
protected:
	virtual int GetCompletion_Implementation(UWorld* World)
	PURE_VIRTUAL(UQuestObjective::GetCompletion_Implementation, return 0;)

	virtual int GetTargetValue_Implementation()
	PURE_VIRTUAL(UQuestObjective::GetTargetValue_Implementation, return 1;)
	
	virtual int TriggerProgress_Implementation(UWorld* World, UBaseQuestEvent* Event)
	PURE_VIRTUAL(UQuestObjective::TriggerProgress_Implementation, return 0;)

protected:
	/**
	 * Can the objective be completed before this quest objective start and hence be checked right after it started.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Quest, meta = (AllowPrivateAccess = true))
	FText ObjectiveDescription;

	/**
	 * Can the objective be completed before this quest objective start and hence be checked right after it started.
	 */
	UPROPERTY(BlueprintReadWrite, Category = Quest, meta = (AllowPrivateAccess = true))
	bool bRetroCompletable = true;

	/**
	 * When triggered, should we add the return value of the trigger function to progress
	 * or use it as new the progress value. 
	 */
	UPROPERTY(BlueprintReadWrite, Category = Quest, meta = (AllowPrivateAccess = true))
	bool bShouldAddProgress = true;
};
