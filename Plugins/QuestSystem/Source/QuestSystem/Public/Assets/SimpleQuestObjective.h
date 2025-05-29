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

protected:
	virtual bool IsSatisfied_Implementation(UWorld* World) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	FName TagToCheck;
};
