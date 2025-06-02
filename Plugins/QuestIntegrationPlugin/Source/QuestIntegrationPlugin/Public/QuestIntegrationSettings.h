// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestIntegrationSettings.generated.h"

class UBaseNotificationWidget;

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UQuestIntegrationSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, category = Quest, meta = (AllowPrivateAccess = true))
	TSubclassOf<UBaseNotificationWidget> QuestMessageWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, category = Quest, meta = (AllowPrivateAccess = true))
	FText QuestStartedFormat;
	
	UPROPERTY(EditDefaultsOnly, category = Quest, meta = (AllowPrivateAccess = true))
	FText QuestCompletedFormat;

	UPROPERTY(EditDefaultsOnly, category = Quest, meta = (AllowPrivateAccess = true))
	float QuestMessageDuration;
};
