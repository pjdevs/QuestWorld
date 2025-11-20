// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "QuestObjective.generated.h"

class UBaseQuestEvent;

UENUM(BlueprintType)
enum EQuestObjectiveType : uint8
{
	Manual = 0,
	Counter = 1,
};

/**
 * Objective representation to use in quest data assets.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class QUESTSYSTEM_API UQuestObjective : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * The current objective progression to TargetValue. Only for retro completable objectives.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Quest)
	int GetCompletion(UWorld* World);

	/**
	 * The target progression value to complete this objective.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Quest)
	int GetTargetValue();
	
	/**
	 * Try progress this objective with the given.
	 * Returns how much progress has been made or 0 if none.
	 * Will use the return value as new progress if bShouldAddProgress instead of adding.
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

public:
	/**
	 * Id of the objective.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Quest)
	FGameplayTag ObjectiveId;

	/**
	 * Can the objective be completed before this quest objective start and hence be checked right after it started.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Quest)
	TEnumAsByte<EQuestObjectiveType> ObjectiveType;
	
	/**
	 * Can the objective be completed before this quest objective start and hence be checked right after it started.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Quest)
	FText ObjectiveDescription;

	/**
	 * Can the objective be completed before this quest objective start and hence be checked right after it started.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Quest)
	bool bIsRetroCompletable = true;

	/**
	 * When triggered, should we add the return value of the trigger function to progress
	 * or use it as new the progress value. 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Quest)
	bool bShouldAddProgress = true;
};
