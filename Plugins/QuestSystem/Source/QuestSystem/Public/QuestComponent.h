// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FastQuestDescriptionArray.h"
#include "QuestDescription.h"
#include "QuestId.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestEventDynamicDelegate, const FQuestDescription&, QuestDescription);


UCLASS(ClassGroup=(Quest), meta=(BlueprintSpawnableComponent))
class QUESTSYSTEM_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestComponent();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	// Functions to access replicated quests
	
	UFUNCTION(BlueprintPure, Category = "Quest")
	TArray<FQuestDescription> GetActiveQuests() const;
	
	UFUNCTION(BlueprintPure, Category = "Quest")
	TArray<FQuestDescription> GetCompletedQuests() const;

	// Delegates
	// TODO also use FFastArraySerializer to replicate only needed quests
	
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FQuestEventDynamicDelegate OnQuestStarted;
	
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FQuestEventDynamicDelegate OnQuestCompleted;
	
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FQuestEventDynamicDelegate OnQuestUpdated;

private:
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

	// Callbacks

	void OnQuestStartedServer(const FQuestId& StartedQuestId);
	void OnQuestCompletedServer(const FQuestId& CompletedQuestId);
	void OnQuestUpdatedServer(const FQuestId& UpdatedQuestId);

private:
	/**
	 * Replicated array of currently active quests.
	 */
	UPROPERTY(ReplicatedUsing=OnRep_ActiveQuests)
	FFastQuestDescriptionArray ActiveQuests;

	/**
	 * Replicated array of already completed quests.
	 */
	UPROPERTY(ReplicatedUsing=OnRep_CompletedQuests)
	FFastQuestDescriptionArray CompletedQuests;

	bool bActiveQuestsReceived;
	bool bCompletedQuestsReceived;
};
