// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestState.h"
#include "ISpudObject.h"
#include "QuestDescription.h"
#include "QuestId.h"
#include "QuestSave.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestSubsystem.generated.h"

class UFlowAsset;
class UBaseQuestEvent;

DECLARE_MULTICAST_DELEGATE_OneParam(FQuestEventDelegate, const FQuestId& /* QuestId */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FQuestCompletedEventDelegate, const FQuestId& /* QuestId */, EQuestCompletionState /* CompletionState */);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FQuestObjectiveCompletedDelegate, const FQuestId& /* QuestId */, const FName& /* ObjectiveId */, EQuestObjectiveCompletionState /* CompletionState */);

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

public: // Spud
	virtual void SpudPostRestore_Implementation(const USpudState* State) override;
	virtual void SpudPreStore_Implementation(const USpudState* State) override;

public: // QuestSubsystem public interface

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void StartQuest(FQuestId QuestId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void SucceedQuest(FQuestId QuestId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void FailQuest(const FQuestId& QuestId);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void StartObjective(FQuestId QuestId, const FName& ObjectiveId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void SucceedObjective(FQuestId QuestId, const FName& ObjectiveId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void FailObjective(FQuestId QuestId, const FName& ObjectiveId);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void ProgressObjective(FQuestId QuestId, const FName& ObjectiveId, int Progress);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void SubmitQuestEvent(UBaseQuestEvent* Event);

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	TArray<FQuestId>GetKnownQuests() const;

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	bool IsQuestStarted(const FQuestId& QuestId) const;

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	bool IsQuestCompleted(const FQuestId& QuestId) const;

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	bool IsObjectiveCompleted(const FQuestId& QuestId, const FName& ObjectiveId) const;

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	FQuestDescription GetQuestDescription(const FQuestId& QuestId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void LoadQuestSave(const FQuestSaveData& QuestSave);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void RestoreQuestsFromSave();

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	FQuestSaveData MakeQuestSave() const;

	UFUNCTION(BlueprintPure, BlueprintAuthorityOnly, Category = "Quest")
	FQuestId GetQuestIdFromFlow(UFlowAsset* QuestFlowInstance) const;

	// Asset management

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	UQuestDataAsset* GetQuestAsset(const FQuestId& QuestId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest")
	void UnloadAll();

public: // Delegates
	FQuestEventDelegate OnQuestStarted;
	FQuestEventDelegate OnQuestCompleted;
	FQuestEventDelegate OnQuestUpdated;
	FQuestObjectiveCompletedDelegate OnObjectiveCompleted;

private:
	void CompleteQuest(const FQuestId& QuestId, EQuestCompletionState CompletionState);
	void CompleteObjective(
		const FQuestId& QuestId,
		const FName& ObjectiveId,
		EQuestObjectiveCompletionState CompletionState
	);

	void HandleQuestCompleted(FQuestState& QuestState);
	
	void StartListeningQuestEvents(FQuestState& QuestState) const;
	void StopListeningQuestEvents(FQuestState& QuestState) const;

	UFUNCTION()
	void OnSpudPostLoadGame(const FString& SlotName, bool bSuccess);

private:
	UPROPERTY()
	TMap<FQuestId, TObjectPtr<UQuestDataAsset>> QuestAssetsById;
	
	TMap<FQuestId, FQuestState> QuestStatesById;

	UPROPERTY()
	TMap<FQuestId, TObjectPtr<UFlowAsset>> QuestFlowsById;

	UPROPERTY(SaveGame)
	FQuestSaveData SpudQuestSaveData;
	
	FQuestSaveData LoadedQuestSaveData;
	bool bHasSaveBeenLoaded;
};
