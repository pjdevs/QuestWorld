// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogEdGraphNode.h"
#include "EdGraph/EdGraphNode.h"
#include "SingleDialogEdGraphNode.generated.h"


/**
 * 
 */
UCLASS()
class DIALOGPLUGINEDITOR_API USingleDialogEdGraphNode : public UDialogEdGraphNode
{
	GENERATED_BODY()

public:
	USingleDialogEdGraphNode();
		
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return DialogLineText; }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Red; }
	virtual void AllocateDefaultPins() override;

	virtual UEdGraphPin* GetDialogInputPin() const override { return InputPin; }
	UEdGraphPin* GetDialogOutputPin() const { return OutputPin; }

public:
	UPROPERTY(EditAnywhere, Category = "Dialog")
	FText DialogLineText;
	
private:
	UEdGraphPin* InputPin;
	UEdGraphPin* OutputPin;
};
