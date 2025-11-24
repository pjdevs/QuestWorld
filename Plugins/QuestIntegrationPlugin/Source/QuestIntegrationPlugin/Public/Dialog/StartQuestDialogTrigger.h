// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogTrigger.h"
#include "QuestId.h"
#include "StartQuestDialogTrigger.generated.h"

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UStartQuestDialogTrigger : public UDialogTrigger
{
	GENERATED_BODY()

public:
	virtual void OnExecute_Implementation(AController* DialogController) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Dialog, meta = (AllowPrivateAccess = true))
	FQuestId QuestToStartId;
};
