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

	UPROPERTY(EditDefaultsOnly, Category = Dialog, meta = (AllowPrivateAccess = true))
	TArray<FText> DialogChoices;

public:
	const TArray<FText>& GetChoices() const { return DialogChoices; }	
};
