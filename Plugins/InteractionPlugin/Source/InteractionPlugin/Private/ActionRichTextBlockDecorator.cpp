// Copyright pjdevs. All Rights Reserved.

#include "ActionRichTextBlockDecorator.h"
#include "InputMappingContext.h"
#include "IPActionWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"

TObjectPtr<const UInputAction> FindInputActionByName(
	const UInputMappingContext* MappingContext,
	const FString& ActionNameToFind
);

class FActionDecorator : public FRichTextDecorator
{
public:
	explicit FActionDecorator(
		URichTextBlock* InOwner,
		TSubclassOf<UIPActionWidget> InActionWidgetClass,
		TObjectPtr<UInputMappingContext> InMappingContext
	)
		: FRichTextDecorator(InOwner),
	      OwnerTextBlock(InOwner),
	      ActionWidgetClass(InActionWidgetClass),
		  MappingContext(InMappingContext)
	{
	}

	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		return RunParseResult.Name == TEXT("Action");
	}

	virtual TSharedPtr<SWidget> CreateDecoratorWidget(
		const FTextRunInfo& RunInfo,
		const FTextBlockStyle& DefaultTextStyle
	) const override
	{
		const FString* ActionNamePtr = RunInfo.MetaData.Find(TEXT("name"));
		const FString ActionName = ActionNamePtr != nullptr ? *ActionNamePtr : TEXT("Default");
		const TObjectPtr<const UInputAction> InputAction = FindInputActionByName(MappingContext, ActionName);
		
		UIPActionWidget* ActionWidget = CreateWidget<UIPActionWidget>(OwnerTextBlock, ActionWidgetClass);

		if (InputAction)
		{
			ActionWidget->SetAction(InputAction);
		}
		
		return ActionWidget != nullptr
			? ActionWidget->TakeWidget().ToSharedPtr()
			: nullptr;
	}

private:
	TObjectPtr<URichTextBlock> OwnerTextBlock;
	TSubclassOf<UIPActionWidget> ActionWidgetClass;
	TObjectPtr<UInputMappingContext> MappingContext;
};

TSharedPtr<ITextDecorator> UActionRichTextBlockDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FActionDecorator(InOwner, ActionWidgetClass, MappingContext));
}

TObjectPtr<const UInputAction> FindInputActionByName(
	const UInputMappingContext* MappingContext,
	const FString& ActionNameToFind
)
{
	if (!MappingContext)
	{
		return nullptr;
	}
	
	for (auto&& Mapping : MappingContext->GetMappings())
	{
		const FString& ActionName = Mapping.Action->GetName();
		if (ActionName.Equals(ActionNameToFind))
		{
			return Mapping.Action;
		}
	}

	return nullptr;
}
