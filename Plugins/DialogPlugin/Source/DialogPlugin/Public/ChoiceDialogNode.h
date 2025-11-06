// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogNode.h"
#include "ChoiceDialogNode.generated.h"

class UDialogChoice;
/**
 * 
 */
UCLASS()
class DIALOGPLUGIN_API UChoiceDialogNode : public UDialogNode
{
	GENERATED_BODY()

public:
	const TArray<TObjectPtr<UDialogChoice>>& GetDialogChoices() const { return DialogChoices; }
	void AddDialogChoice(UDialogChoice* DialogChoice) { DialogChoices.Add(DialogChoice); }

	virtual bool IsAvailable(UWorld* World, AController* DialogController) const override;

private:
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Dialog, meta = (AllowPrivateAccess = true))
	TArray<TObjectPtr<UDialogChoice>> DialogChoices;
};
