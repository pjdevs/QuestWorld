// Copyright pjdevs. All Rights Reserved.


#include "DialogComponent.h"

#include "ChoiceDialogNode.h"
#include "DialogDataAsset.h"
#include "DialogNode.h"
#include "DialogWidget.h"
#include "SingleDialogNode.h"
#include "Blueprint/UserWidget.h"


UDialogComponent::UDialogComponent()
{
	DisplayLineFinishedDelegate.BindDynamic(this, &UDialogComponent::OnLineDisplayed);
	DisplayChoicesFinishedDelegate.BindDynamic(this, &UDialogComponent::OnChoicesDisplayed);
}

void UDialogComponent::StartDialog(UDialogDataAsset* DialogAsset)
{
	DisplayedDialogWidget = Cast<UDialogWidget>(CreateWidget(GetOwner(), DialogWidgetClass));
	DisplayedDialogWidget->AddToViewport();
	
	CurrentNode = DialogAsset->GetDialogRoot();
	ExecuteCurrentDialogNode();
}

void UDialogComponent::ExecuteCurrentDialogNode()
{
	if (!CurrentNode || !CurrentNode->IsAvailable())
	{
		EndDialog();
		return;
	}

	if (const USingleDialogNode* SingleDialogNode = Cast<USingleDialogNode>(CurrentNode))
	{
		DisplayedDialogWidget->DisplayLine(SingleDialogNode->GetLine(), DisplayLineFinishedDelegate);
	}
	else if (const UChoiceDialogNode* ChoiceDialogNode = Cast<UChoiceDialogNode>(CurrentNode))
	{
		DisplayedDialogWidget->DisplayChoices(ChoiceDialogNode->GetChoices(), DisplayChoicesFinishedDelegate);
	}
	else
	{
		EndDialog();
	}
}

void UDialogComponent::EndDialog()
{
	CurrentNode = nullptr;
	
	if (DisplayedDialogWidget)
	{
		DisplayedDialogWidget->RemoveFromParent();
		DisplayedDialogWidget = nullptr;
	}
}

void UDialogComponent::OnLineDisplayed()
{
	if (CurrentNode && CurrentNode->GetNextDialogs().Num() > 0)
	{
		CurrentNode = CurrentNode->GetNextDialogs()[0];
		ExecuteCurrentDialogNode();
	}
	else
	{
		EndDialog();
	}
}

void UDialogComponent::OnChoicesDisplayed(int ChoiceIndex)
{
	if (CurrentNode)
	{
		const auto& Choices = CurrentNode->GetNextDialogs();

		if (Choices.Num() > 0 && ChoiceIndex >= 0 && ChoiceIndex < Choices.Num())
		{
			CurrentNode = Choices[ChoiceIndex];
			ExecuteCurrentDialogNode();
			return;
		}
	}

	EndDialog();
}
