// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogEdGraphNode.h"
#include "EdGraph/EdGraphNode.h"
#include "ChoiceDialogEdGraphNode.generated.h"


/**
 * 
 */
UCLASS()
class DIALOGPLUGINEDITOR_API UChoiceDialogEdGraphNode : public UDialogEdGraphNode
{
	GENERATED_BODY()

public:
	UChoiceDialogEdGraphNode();
		
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString("Dialog Choice"); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Green; }
	virtual void AllocateDefaultPins() override;

	virtual UEdGraphPin* GetDialogInputPin() const override { return InputPin; }
	const TArray<UEdGraphPin*>& GetNextDialogsPins() const { return ChoicePins; }

	const UDialogEdGraphNode* GetNextNodeForChoice(int ChoiceIndex) const;

public:
	UPROPERTY(EditAnywhere, Category = "Dialog")
	TArray<FText> DialogChoices;
	
private:
	UEdGraphPin* InputPin;
	TArray<UEdGraphPin*> ChoicePins;
};
