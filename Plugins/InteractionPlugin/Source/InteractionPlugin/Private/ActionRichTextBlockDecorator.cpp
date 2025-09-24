// Copyright pjdevs. All Rights Reserved.

#include "ActionRichTextBlockDecorator.h"
#include "IPActionWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"

class FActionDecorator : public FRichTextDecorator
{
public:
	explicit FActionDecorator(URichTextBlock* InOwner, TSubclassOf<UIPActionWidget> InActionWidgetClass)
		: FRichTextDecorator(InOwner), OwnerTextBlock(InOwner), ActionWidgetClass(InActionWidgetClass)
	{}

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
		
		UIPActionWidget* ActionWidget = CreateWidget<UIPActionWidget>(OwnerTextBlock, ActionWidgetClass);
		ActionWidget->SetAction(ActionName);
		
		return ActionWidget->TakeWidget();
	}

private:
	TObjectPtr<URichTextBlock> OwnerTextBlock;
	TSubclassOf<UIPActionWidget> ActionWidgetClass;
};

TSharedPtr<ITextDecorator> UActionRichTextBlockDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FActionDecorator(InOwner, ActionWidgetClass));
}