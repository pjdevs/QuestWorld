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
	virtual int GetCompletion_Implementation(UWorld* World) const override;
	virtual int GetTargetValue_Implementation() const override;
	virtual int TriggerProgress_Implementation(UWorld* World, UBaseQuestEvent* Event) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action", meta = (AllowPrivateAccess = "true"))
	FName ActionToDo;

	UPROPERTY(EditDefaultsOnly, Category = "Action", meta = (AllowPrivateAccess = "true"))
	int TimesToDoAction;
};
