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
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UQuestIntegrationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UQuestIntegrationSettings> QuestIntegrationSettings;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void SetQuestIntegrationSettings(UQuestIntegrationSettings* Settings);
	
private:
	UFUNCTION()
	void OnQuestStarted(const FQuestDescription& Quest);
	UFUNCTION()
	void OnQuestCompleted(const FQuestDescription& Quest);
};
