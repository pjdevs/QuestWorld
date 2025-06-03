// Copyright pjdevs. All Rights Reserved.


#include "DialogComponent.h"

#include "ChoiceDialogNode.h"
#include "DialogDataAsset.h"
#include "DialogNode.h"
#include "DialogWidget.h"
#include "SingleDialogNode.h"
#include "Blueprint/UserWidget.h"


void UDialogComponent::StartDialog(UDialogDataAsset* DialogAsset)
{
	APlayerController* OwnerController = Cast<APlayerController>(GetOwner());

	if (!OwnerController)
		return;
	
	DisplayedDialogWidget = Cast<UDialogWidget>(CreateWidget(OwnerController, DialogWidgetClass));
	DisplayedDialogWidget->AddToViewport();
	DisplayedDialogWidget->DisplayLineFinishedDelegate.BindUObject(this, &UDialogComponent::OnLineDisplayed);
	DisplayedDialogWidget->DisplayChoicesFinishedDelegate.BindUObject(this, &UDialogComponent::OnChoicesDisplayed);

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

	CurrentNode->Trigger();

	if (const USingleDialogNode* SingleDialogNode = Cast<USingleDialogNode>(CurrentNode))
	{
		DisplayedDialogWidget->DisplayLine(SingleDialogNode->GetLine());
	}
	else if (const UChoiceDialogNode* ChoiceDialogNode = Cast<UChoiceDialogNode>(CurrentNode))
	{
		DisplayedDialogWidget->DisplayChoices(ChoiceDialogNode->GetChoices());
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
		DisplayedDialogWidget->DisplayLineFinishedDelegate.Unbind();
		DisplayedDialogWidget->DisplayChoicesFinishedDelegate.Unbind();
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
