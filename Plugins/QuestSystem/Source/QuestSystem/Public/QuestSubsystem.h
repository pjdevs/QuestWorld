// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "QuestService.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestSubsystem.generated.h"

class UBaseQuestEvent;
DECLARE_DYNAMIC_DELEGATE(FQuestLoadedDynamicDelegate);

UCLASS()
class UQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = Quest)
	void LoadQuests(const FQuestLoadedDynamicDelegate CompletionDelegate);

	UFUNCTION(BlueprintCallable, Category = Quest)
	void StartQuest(const FPrimaryAssetId& QuestId);

	UFUNCTION(BlueprintCallable, Category = Quest)
	TArray<FPrimaryAssetId> GetActiveQuests() const;

	UFUNCTION(BlueprintCallable, Category = Quest)
	TArray<FPrimaryAssetId> GetCompletedQuests() const;

	UFUNCTION(BlueprintCallable, Category = Quest)
	void SubmitQuestEvent(UBaseQuestEvent* Event);
	
private:
	UPROPERTY()
	TScriptInterface<IQuestService> QuestService;
};
