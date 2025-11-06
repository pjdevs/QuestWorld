// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogNode.generated.h"

class UDialogTrigger;
class UDialogCondition;

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class DIALOGPLUGIN_API UDialogNode : public UObject
{
	GENERATED_BODY()

public:
	const TArray<TObjectPtr<UDialogCondition>>& GetConditions() const { return Conditions; }
	const TArray<TObjectPtr<UDialogTrigger>>& GetTriggers() const { return Triggers; }
	
	void AddCondition(UDialogCondition* Condition) { Conditions.Add(Condition); }
	void AddTrigger(UDialogTrigger* Trigger) { Triggers.Add(Trigger); }
	
	virtual bool IsAvailable(UWorld* World, AController* DialogController) const;
	void Trigger(UWorld* World, AController* DialogController);

#if WITH_EDITORONLY_DATA
public:
	UPROPERTY()
	FVector2f EditorNodePosition;
#endif

private:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Dialog, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UDialogCondition>> Conditions;
	
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Dialog, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UDialogTrigger>> Triggers;

};


