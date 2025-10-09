// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestDescription.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestIntegrationSubsystem.generated.h"

// TODO In this plugin, make a QuestStepCompleted/QuestObjectiveCompleted dialog condition?

class UQuestIntegrationSettings;
class UBaseNotificationWidget;

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig) // temp Game because cannot load custom ini
class QUESTINTEGRATIONPLUGIN_API UQuestIntegrationSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

private:
	UFUNCTION()
	void OnQuestStarted(const FQuestDescription& Quest);
	UFUNCTION()
	void OnQuestCompleted(const FQuestDescription& Quest);

private:
	UPROPERTY(Config)
	TSoftObjectPtr<UQuestIntegrationSettings> QuestIntegrationSettings;
	
private:
	UPROPERTY(Transient)
	TObjectPtr<UQuestIntegrationSettings> CurrentSettings;
};
