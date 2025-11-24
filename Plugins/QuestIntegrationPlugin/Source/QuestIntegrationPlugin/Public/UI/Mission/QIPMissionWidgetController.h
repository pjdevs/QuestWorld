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
	UFUNCTION()
	void InitializeWithGameState(AGameStateBase* InGameState);
	void Cleanup();
	void TriggerInitialEvents();

public:
	FMissionUpdatedDelegate OnMissionUpdated;
	FOnNoMissionDelegate OnNoMission;

private:
	void TryUpdateFirstMission() const;

	UFUNCTION()
	void OnQuestUpdated(const FQuestDescription& QuestDescription);
	
private:
	UPROPERTY()
	TObjectPtr<UQuestComponent> QuestComponent;
};
