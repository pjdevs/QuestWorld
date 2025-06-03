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

	UPROPERTY(EditDefaultsOnly, Instanced, Category = Dialog, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UDialogNode>> NextDialogs;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = Dialog, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UDialogCondition>> Conditions;
	
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Dialog, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UDialogTrigger>> Triggers;

public:
	const TArray<TObjectPtr<UDialogNode>>& GetNextDialogs() const { return NextDialogs; }

	bool IsAvailable() const;
	void Trigger();
};


