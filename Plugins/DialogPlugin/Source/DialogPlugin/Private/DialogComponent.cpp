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
	DisplayedDialogWidget->DisplayLineFinishedDelegate.BindUObject(this, &UDialogComponent::OnLineDisplayed);
	DisplayedDialogWidget->DisplayChoicesFinishedDelegate.BindUObject(this, &UDialogComponent::OnChoicesDisplayed);
	DisplayedDialogWidget->AddToViewport();
	DisplayedDialogWidget->SetCharacterName(DialogAsset->GetCharacterName());

	CurrentNode = DialogAsset->GetDialogRoot();
	ExecuteCurrentDialogNode();
}

TArray<FText> SetAvailableChoiceIndexes(
	UWorld* World,
	const UChoiceDialogNode* ChoiceDialogNode,
	TArray<int> AvailableIndexes
)
{
	const TArray<FText>& AllChoices = ChoiceDialogNode->GetChoices();
	TArray<FText> AvailableChoices;

	if (AllChoices.Num() != ChoiceDialogNode->GetNextDialogs().Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Choice node should have same number of choices and child nodes"));
		return AvailableChoices;
	}
	
	AvailableIndexes.Empty();

	auto& Children = ChoiceDialogNode->GetNextDialogs();

	for (int i = 0; i < AllChoices.Num(); i++)
	{
		const UDialogNode* Child = Children[i];
		const FText& Choice = AllChoices[i];
			
		if (!Child->IsAvailable(World))
			continue;
		
		AvailableChoices.Add(Choice);
		AvailableIndexes.Add(i);
	}

	return AvailableChoices;
}

void UDialogComponent::ExecuteCurrentDialogNode()
{
	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("No world while dialog"));
		return;
	}
	
	if (!CurrentNode || !CurrentNode->IsAvailable(GetWorld()))
	{
		EndDialog();
		return;
	}

	CurrentNode->Trigger(World);

	if (const USingleDialogNode* SingleDialogNode = Cast<USingleDialogNode>(CurrentNode))
	{
		DisplayedDialogWidget->DisplayLine(SingleDialogNode->GetLine());
	}
	else if (const UChoiceDialogNode* ChoiceDialogNode = Cast<UChoiceDialogNode>(CurrentNode))
	{
		TArray<FText> AvailableChoices = SetAvailableChoiceIndexes(World, ChoiceDialogNode, AvailableChoiceIndexes);

		if (AvailableChoices.Num() <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Available choice node should have at least one available choice."));
			EndDialog();
			return;
		}
		
		DisplayedDialogWidget->DisplayChoices(AvailableChoices);
	}
	else
	{
		TryGoToChildNode(0);
	}
}

void UDialogComponent::TryGoToChildNode(int NodeIndex)
{
	if (CurrentNode)
	{
		const auto& Choices = CurrentNode->GetNextDialogs();

		if (Choices.Num() > 0 && NodeIndex >= 0 && NodeIndex < Choices.Num())
		{
			CurrentNode = Choices[NodeIndex];
			ExecuteCurrentDialogNode();
			return;
		}
	}

	EndDialog();
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
	TryGoToChildNode(0);
}

void UDialogComponent::OnChoicesDisplayed(int ChoiceIndex)
{
	if (ChoiceIndex >= 0 && ChoiceIndex < AvailableChoiceIndexes.Num())
	{
		EndDialog();
		return;
	}

	const int RealChoiceIndex = AvailableChoiceIndexes[ChoiceIndex];
	TryGoToChildNode(RealChoiceIndex);
}
