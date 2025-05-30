// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestObjective.h"
#include "SimpleQuestObjective.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API USimpleQuestObjective : public UQuestObjective
{
	GENERATED_BODY()

public:
	USimpleQuestObjective();

protected:
	virtual int GetCompletion_Implementation(UWorld* World) override;
	virtual int GetTargetValue_Implementation() override;
	virtual int TriggerProgress_Implementation(UWorld* World, UBaseQuestEvent* Event) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	FName TagToCheck;
};
