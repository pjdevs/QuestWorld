// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestDescription.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"


class UBaseQuestEvent;
class IQuestService;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FQuestsLoadedDynamicDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestEventDynamicDelegate, const FQuestDescription&, Quest);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTSYSTEM_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UQuestComponent();

	UFUNCTION(BlueprintCallable, Category = Quest)
	void StartQuest(FPrimaryAssetId QuestId);

	UFUNCTION(BlueprintCallable, Category = Quest)
	TArray<FQuestDescription> GetActiveQuests() const;

	UFUNCTION(BlueprintCallable, Category = Quest)
	TArray<FQuestDescription> GetCompletedQuests() const;

	UFUNCTION(BlueprintCallable, Category = Quest)
	bool IsQuestCompleted(FPrimaryAssetId QuestId) const;

	UFUNCTION(BlueprintCallable, Category = Quest)
	bool IsQuestActive(FPrimaryAssetId QuestId) const;
	
	UFUNCTION(BlueprintCallable, Category = Quest)
	void SubmitQuestEvent(UBaseQuestEvent* Event);

	UPROPERTY(BlueprintAssignable, Category = Quest)
	FQuestsLoadedDynamicDelegate OnQuestsLoaded;

	UPROPERTY(BlueprintAssignable, Category = Quest)
	FQuestEventDynamicDelegate OnQuestStarted;
	
	UPROPERTY(BlueprintAssignable, Category = Quest)
	FQuestEventDynamicDelegate OnQuestCompleted;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(Transient)
	TScriptInterface<IQuestService> QuestService;
};
