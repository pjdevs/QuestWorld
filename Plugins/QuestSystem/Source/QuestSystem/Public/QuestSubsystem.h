// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "QuestService.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE(FQuestLoadedDynamicDelegate);

UCLASS()
class UQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void LoadQuests(const FQuestLoadedDynamicDelegate CompletionDelegate) const;

	UFUNCTION(BlueprintCallable)
	void StartQuest(const FName QuestId, APlayerController* PlayerController) const;

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetActiveQuests() const;

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetCompletedQuests() const;

private:
	UPROPERTY()
	TScriptInterface<IQuestService> QuestService;
};
