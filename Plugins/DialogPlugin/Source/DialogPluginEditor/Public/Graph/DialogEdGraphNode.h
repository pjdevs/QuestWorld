// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogEdGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGPLUGINEDITOR_API UDialogEdGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Dialog", meta = (AllowPrivateAccess = true))
	FText SpeakerText;
};
