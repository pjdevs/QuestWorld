// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"

#include "ActionRichTextBlockDecorator.generated.h"

class UIPActionWidget;

/**
 * Custom rich text box decorator to insert action widget inside.
 */
UCLASS()
class INTERACTIONPLUGIN_API UActionRichTextBlockDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

protected:
	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TSubclassOf<UIPActionWidget> ActionWidgetClass;
};
