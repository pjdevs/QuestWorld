// Copyright pjdevs. All Rights Reserved.


#include "DialogComponent.h"

#include "ChoiceDialogNode.h"
#include "DialogChoice.h"
#include "DialogEvents.h"
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
	// Check if dialog is already active
	if (CurrentDialogActor)
	{
		return;
	}
	
	// Check if the thing that component is attached on a player
	APlayerController* PlayerController = Cast<APlayerController>(OwnerController);
	
	if (!PlayerController)
	{
		return;
	}

	// Store current actor which we are talking with and call start if implementing interface
	CurrentDialogActor = DialogActor;

	if (IDialogEvents* DialogEvents = Cast<IDialogEvents>(DialogActor))
	{
		DialogEvents->OnDialogStarted(OwnerController);
	}

	// Prevent player moving while dialog is active
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
	const TArray<TObjectPtr<UDialogChoice>>& AllChoices = ChoiceDialogNode->GetDialogChoices();
	TArray<FText> AvailableChoices;
	
	AvailableIndexes.Empty();

	for (int i = 0; i < AllChoices.Num(); i++)
	{
		const UDialogChoice* Choice = AllChoices[i];
			
		if (Choice->GetNextDialog() && !Choice->GetNextDialog()->IsAvailable(World))
		{
			continue;
		}
		
		AvailableChoices.Add(Choice->GetChoiceText());
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
	
	if (!CurrentNode || !CurrentNode->IsAvailable(World))
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
	if (const USingleDialogNode* SingleDialogNode = Cast<USingleDialogNode>(CurrentNode))
	{
		CurrentNode = SingleDialogNode->GetNextDialog();
		ExecuteCurrentDialogNode();
		return;
	}

	if (const UChoiceDialogNode* ChoiceDialogNode = Cast<UChoiceDialogNode>(CurrentNode))
	{
		auto& Choices = ChoiceDialogNode->GetDialogChoices();

		if (Choices.Num() > 0 && NodeIndex >= 0 && NodeIndex < Choices.Num())
		{
			CurrentNode = Choices[NodeIndex]->GetNextDialog();
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

	if (IDialogEvents* DialogEvents = Cast<IDialogEvents>(CurrentDialogActor))
	{
		DialogEvents->OnDialogEnded(OwnerController);
	}

	CurrentDialogActor = nullptr;
	
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
