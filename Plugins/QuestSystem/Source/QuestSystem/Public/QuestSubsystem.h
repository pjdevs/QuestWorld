// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveQuest.h"
#include "ISpudObject.h"
#include "QuestDescription.h"
#include "QuestId.h"
#include "QuestSaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestSubsystem.generated.h"

class UFlowAsset;
class UBaseQuestEvent;

DECLARE_DELEGATE(FQuestLoadedDelegate);
DECLARE_DELEGATE_OneParam(FQuestEventDelegate, const FQuestId&);

/**
 * Core subsystem of the quest system. Only relevant on server.
 * For UI replication of quests state, see QuestComponent.
 */
UCLASS()
class QUESTSYSTEM_API UQuestSubsystem : public UGameInstanceSubsystem, public ISpudObject, public ISpudObjectCallback
{
	GENERATED_BODY()

public: // Subsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

public: // Spud
	virtual void SpudPostRestore_Implementation(const USpudState* State) override;
	virtual void SpudPreStore_Implementation(const USpudState* State) override;
	
public: // QuestSubsystem public interface

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void StartQuest(FQuestId QuestId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void CompleteQuest(FQuestId QuestId);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void SubmitQuestEvent(UBaseQuestEvent* Event);

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	TArray<FQuestId> GetActiveQuests() const;

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	TArray<FQuestId> GetCompletedQuests() const;

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	bool IsQuestActive(const FQuestId& QuestId) const { return ActiveQuestsById.Contains(QuestId); }

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	bool IsQuestCompleted(const FQuestId& QuestId) const { return CompletedQuestIds.Contains(QuestId); }

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	FQuestDescription GetQuestDescription(const FQuestId& QuestId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void RestoreQuests(const FQuestSaveData& QuestSave);

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	FQuestSaveData GetQuestSave() const;
	
public: // Delegates
	FQuestEventDelegate OnQuestStarted;
	FQuestEventDelegate OnQuestCompleted;
	FQuestEventDelegate OnQuestUpdated;

private:
	UQuestDataAsset* GetQuestAsset(const FQuestId& QuestId);
	void UnloadAll();

private:
	FPrimaryAssetType QuestAssetType;

	UPROPERTY()
	TMap<FQuestId, TObjectPtr<UQuestDataAsset>> QuestAssetsById;
	TMap<FQuestId, FActiveQuest> ActiveQuestsById;
	TMap<FQuestId, TObjectPtr<UFlowAsset>> ActiveQuestFlowsById;
	TArray<FQuestId> CompletedQuestIds;
	
	UPROPERTY(SaveGame)
	FQuestSaveData SpudQuestSaveData;
};
