// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogTrigger.h"
#include "StartQuestDialogTrigger.generated.h"

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UStartQuestDialogTrigger : public UDialogTrigger
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	FPrimaryAssetId QuestToStartId;

public:
	virtual void Execute_Implementation() override;
};
