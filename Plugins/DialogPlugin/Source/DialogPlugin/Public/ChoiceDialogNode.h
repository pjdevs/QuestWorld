// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogNode.h"
#include "ChoiceDialogNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGPLUGIN_API UChoiceDialogNode : public UDialogNode
{
	GENERATED_BODY()

public:
	const TArray<FText>& GetChoices() const { return DialogChoices; }
	void SetChoices(const TArray<FText>& Choices) { DialogChoices = Choices; }

	virtual bool IsAvailable(UWorld* World) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Dialog, meta = (AllowPrivateAccess = true))
	TArray<FText> DialogChoices;
};
