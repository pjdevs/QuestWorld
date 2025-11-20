// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ISpudObject.h"
#include "QuestDescription.h"
#include "QuestSaveGame.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"


class UBaseQuestEvent;
class IQuestService;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuestDynamicDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestEventDynamicDelegate, const FQuestDescription&, Quest);

UCLASS(ClassGroup=(Quest), meta=(BlueprintSpawnableComponent))
class QUESTSYSTEM_API UQuestComponent : public UActorComponent, public ISpudObject, public ISpudObjectCallback
{
	GENERATED_BODY()

public:
	UQuestComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public: // Spud
	virtual void SpudPostRestore_Implementation(const USpudState* State) override;
	virtual void SpudPreStore_Implementation(const USpudState* State) override;
	
public:
	// All server functions that interact with the service

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Quest)
	void StartQuest(FPrimaryAssetId QuestId);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Quest)
	void SubmitQuestEvent(UBaseQuestEvent* Event);

	// Functions to access replicated quests
	
	UFUNCTION(BlueprintPure, Category = Quest)
	TArray<FQuestDescription> GetActiveQuests() const;
	
	UFUNCTION(BlueprintPure, Category = Quest)
	TArray<FQuestDescription> GetCompletedQuests() const;

	UFUNCTION(BlueprintPure, Category = Quest)
	bool IsQuestCompleted(FPrimaryAssetId QuestId) const;

	UFUNCTION(BlueprintPure, Category = Quest)
	bool IsQuestActive(FPrimaryAssetId QuestId) const;

	// Save functions, server only

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest|Save")
	void LoadQuestsFromSave(const FQuestSaveData& QuestSaveData);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Quest|Save")
	FQuestSaveData WriteQuestsToSave() const;

	// Delegates

	/**
	 * Client only delegate to know when quest array is received from server.
	 */
	UPROPERTY(BlueprintAssignable, Category = Quest)
	FQuestDynamicDelegate OnActiveQuestsUpdated;

	/**
	 * Client only delegate to know when quest array is received from server.
	 */
	UPROPERTY(BlueprintAssignable, Category = Quest)
	FQuestDynamicDelegate OnCompletedQuestsUpdated;

	/**
	 * Sever only delegate called when quests are loaded.
	 */
	UPROPERTY(BlueprintAssignable, Category = Quest)
	FQuestDynamicDelegate OnQuestsLoaded;

	// Shared delegate called both on server and client

	// TODO later make multicast calls to updated only some quests
	// TODO also use FFastArraySerializer to replicate only needed quests
	
	UPROPERTY(BlueprintAssignable, Category = Quest)
	FQuestEventDynamicDelegate OnQuestStarted;
	
	UPROPERTY(BlueprintAssignable, Category = Quest)
	FQuestEventDynamicDelegate OnQuestCompleted;
	
	UPROPERTY(BlueprintAssignable, Category = Quest)
	FQuestEventDynamicDelegate OnQuestUpdated;

private:
	/**
	 * Init the quest service and set delegates. Only called on server
	 */
	void InitQuestService();

	// Callback for service events
	
	void OnQuestsLoadedServer();
	void OnQuestStartedServer(const FQuestDescription& StartedQuest);
	void OnQuestCompletedServer(const FQuestDescription& CompletedQuest);
	void OnQuestUpdatedServer(const FQuestDescription& UpdatedQuest);

	// Multicast RPCs to call event delegates on client

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_QuestStarted(const FQuestDescription& StartedQuest);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_QuestCompleted(const FQuestDescription& StartedQuest);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_QuestUpdated(const FQuestDescription& StartedQuest);
	
	/**
	 * Function called when active quest array is replicated on client.
	 */
	UFUNCTION()
	void OnRep_ActiveQuests();

	/**
	 * Function called when completed quest array is replicated on client.
	 */
	UFUNCTION()
	void OnRep_CompletedQuests();

private:
	/**
	 * Server and runtime only service.
	 */
	UPROPERTY(Transient)
	TScriptInterface<IQuestService> QuestService;

	/**
	 * Replicated array of currently active quests.
	 */
	UPROPERTY(ReplicatedUsing=OnRep_ActiveQuests)
	TArray<FQuestDescription> ActiveQuests;

	/**
	 * Replicated array of already completed quests.
	 */
	UPROPERTY(ReplicatedUsing=OnRep_CompletedQuests)
	TArray<FQuestDescription> CompletedQuests;

private:
	UPROPERTY(SaveGame)
	FQuestSaveData QuestSaveData;
};
