#include "DialogWidget.h"

void UDialogWidget::DisplayLineFinished()
{
	bool _ = DisplayLineFinishedDelegate.ExecuteIfBound();
}

void UDialogWidget::DisplayChoicesFinished(int ChoiceIndex)
{
	bool _ = DisplayChoicesFinishedDelegate.ExecuteIfBound(ChoiceIndex);
}
