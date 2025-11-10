// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QIPMissionWidgetController.generated.h"

struct FQuestDescription;
class UQuestComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FMissionUpdatedDelegate, const FQuestDescription& /* CurrentMission */);
DECLARE_MULTICAST_DELEGATE(FOnNoMissionDelegate);

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UQIPMissionWidgetController : public UObject
{
	GENERATED_BODY()

public:
	void InitializeWithGameState(AGameStateBase* InGameState);
	void Cleanup();
	void TriggerInitialEvents();

public:
	FMissionUpdatedDelegate OnMissionUpdated;
	FOnNoMissionDelegate OnNoMission;

private:
	UFUNCTION()
	void OnActiveQuestsUpdated();
	
private:
	UPROPERTY()
	TObjectPtr<UQuestComponent> QuestComponent;
};
