// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "QuestService.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestSubsystem.generated.h"

class UBaseQuestEvent;
DECLARE_DYNAMIC_DELEGATE(FQuestLoadedDynamicDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestEventMulticastDelegate, const FQuestDescription&, Quest);

UCLASS()
class QUESTSYSTEM_API UQuestSubsystem : public UGameInstanceSubsystem
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
	TArray<FQuestDescription> GetActiveQuests() const;

	UFUNCTION(BlueprintCallable, Category = Quest)
	TArray<FQuestDescription> GetCompletedQuests() const;

	UFUNCTION(BlueprintCallable, Category = Quest)
	void SubmitQuestEvent(UBaseQuestEvent* Event);

	UPROPERTY(BlueprintAssignable, Category = Quest)
	FQuestEventMulticastDelegate QuestStartedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = Quest)
	FQuestEventMulticastDelegate QuestCompletedDelegate;
	
private:
	UPROPERTY()
	TScriptInterface<IQuestService> QuestService;
};
