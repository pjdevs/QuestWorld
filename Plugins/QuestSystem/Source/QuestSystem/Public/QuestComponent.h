// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FastQuestDescriptionArray.h"
#include "QuestDescription.h"
#include "QuestId.h"
#include "QuestSubsystem.h"
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	// Functions to access replicated quests
	
	UFUNCTION(BlueprintPure, Category = "Quest")
	TArray<FQuestDescription> GetKnownQuests() const;

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
	void OnRep_KnownQuests();

	// Callbacks

	void OnQuestUpdatedServer(const FQuestId& UpdatedQuestId);

private:
	/**
	 * Replicated array of currently active quests.
	 */
	UPROPERTY(ReplicatedUsing=OnRep_KnownQuests)
	FFastQuestDescriptionArray KnownQuests;

	bool bKnownQuestsReceived;

	FDelegateHandle QuestStartedDelegateHandle;
	FDelegateHandle QuestCompletedDelegateHandle;
	FDelegateHandle QuestUpdatedDelegateHandle;
};
