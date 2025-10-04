// Copyright pjdevs. All Rights Reserved.


#include "DialogComponent.h"

#include "ChoiceDialogNode.h"
#include "DialogGraphAsset.h"
#include "DialogNode.h"
#include "DialogWidget.h"
#include "SingleDialogNode.h"
#include "Blueprint/UserWidget.h"

// TODO One day rethink that also AI etc can dialog an we don't necessary have a widget

void UDialogComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerController = Cast<AController>(GetOwner());
}

void UDialogComponent::StartDialog(AActor* DialogActor, UDialogGraphAsset* DialogAsset)
{
	APlayerController* PlayerController = Cast<APlayerController>(OwnerController);
	
	if (!PlayerController)
		return;

	CurrentDialogActor = DialogActor;

	if (APawn* Pawn = PlayerController->GetPawn())
	{
		Pawn->DisableInput(PlayerController);
	}
	
	DisplayedDialogWidget = CreateWidget<UDialogWidget>(PlayerController, DialogWidgetClass);
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
	TArray<int>& AvailableIndexes
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

	CurrentNode->Trigger(World, OwnerController);

	if (const USingleDialogNode* SingleDialogNode = Cast<USingleDialogNode>(CurrentNode))
	{
		DisplayedDialogWidget->DisplayLine(SingleDialogNode->GetLine());
	}
	else if (const UChoiceDialogNode* ChoiceDialogNode = Cast<UChoiceDialogNode>(CurrentNode))
	{
		const TArray<FText> AvailableChoices = SetAvailableChoiceIndexes(World, ChoiceDialogNode, AvailableChoiceIndexes);

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

	if (APlayerController* PlayerController = Cast<APlayerController>(OwnerController))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			Pawn->EnableInput(PlayerController);
		}
	}
}

void UDialogComponent::OnLineDisplayed()
{
	TryGoToChildNode(0);
}

void UDialogComponent::OnChoicesDisplayed(int ChoiceIndex)
{
	if (ChoiceIndex < 0 || ChoiceIndex >= AvailableChoiceIndexes.Num())
	{
		EndDialog();
		return;
	}

	const int RealChoiceIndex = AvailableChoiceIndexes[ChoiceIndex];
	TryGoToChildNode(RealChoiceIndex);
}
