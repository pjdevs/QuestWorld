// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestObjective.h"
#include "UObject/Object.h"
#include "ActionQuestObjective.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UActionQuestObjective : public UQuestObjective
{
	GENERATED_BODY()
	
public:
	UActionQuestObjective();

protected:
	virtual int GetCompletion_Implementation(UWorld* World) override;
	virtual int GetTargetValue_Implementation() override;
	virtual int TriggerProgress_Implementation(UWorld* World, UBaseQuestEvent* Event) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	FName ActionToDo;

	UPROPERTY(EditDefaultsOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	int TimesToDoAction;
};
