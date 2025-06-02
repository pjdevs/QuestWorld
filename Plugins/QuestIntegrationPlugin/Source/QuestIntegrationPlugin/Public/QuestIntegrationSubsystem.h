// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestDescription.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestIntegrationSubsystem.generated.h"

class UQuestIntegrationSettings;
class UBaseNotificationWidget;

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig) // temp Game because cannot load custom ini
class QUESTINTEGRATIONPLUGIN_API UQuestIntegrationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY(Config)
	TSoftObjectPtr<UQuestIntegrationSettings> QuestIntegrationSettings;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	UFUNCTION()
	void OnQuestStarted(const FQuestDescription& Quest);
	UFUNCTION()
	void OnQuestCompleted(const FQuestDescription& Quest);

private:
	UPROPERTY()
	TObjectPtr<UQuestIntegrationSettings> CurrentSettings;
};
