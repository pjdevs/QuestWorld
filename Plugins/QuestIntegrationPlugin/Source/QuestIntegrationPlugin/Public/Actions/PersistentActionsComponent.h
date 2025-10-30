// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PersistentActionsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTINTEGRATIONPLUGIN_API UPersistentActionsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPersistentActionsComponent();

public:
	UFUNCTION(BlueprintCallable, Category = "PersistentActions")
	void AddActionDone(const FName& ActionName) { ActionsDone.Add(ActionName); }

	UFUNCTION(BlueprintCallable, Category = "PersistentActions")
	const TSet<FName>& GetActionsDone() const { return ActionsDone; }

	UFUNCTION(BlueprintCallable, Category = "PersistentActions")
	void SetActionsDone(const TSet<FName> Actions) { ActionsDone = Actions; } 

private:
	TSet<FName> ActionsDone;
};

