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

	/**
	 * Can the objective be completed before this quest objective start and hence be checked right after it started.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	bool bRetroCompletable;

public:
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool IsRetroCompletable() const { return bRetroCompletable; }
	
	UFUNCTION(BlueprintNativeEvent, Category = "Quest")
	bool IsSatisfied(UWorld* World);

protected:
	virtual bool IsSatisfied_Implementation(UWorld* World)
	PURE_VIRTUAL(UQuestObjective::IsSatisfied_Implementation, return false;);
};
