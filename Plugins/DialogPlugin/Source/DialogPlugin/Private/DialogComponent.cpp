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

UDialogComponent::UDialogComponent()
{
	SetIsReplicatedByDefault(true);
}

void UDialogComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerController = Cast<AController>(GetOwner());
}

void UDialogComponent::StartDialog(AActor* DialogActor, UDialogGraphAsset* DialogAsset)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}
	
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

	Client_CreateDialogWidget(PlayerController, DialogAsset);

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
		Client_DisplayLine(SingleDialogNode->GetLine());
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
		
		Client_DisplayChoices(AvailableChoices);
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

	Client_DestroyDialogWidget();

	if (IDialogEvents* DialogEvents = Cast<IDialogEvents>(CurrentDialogActor))
	{
		DialogEvents->OnDialogEnded(OwnerController);
	}

	CurrentDialogActor = nullptr;
}

void UDialogComponent::OnLineDisplayedServer()
{
	TryGoToChildNode(0);
}

void UDialogComponent::OnChoicesDisplayedServer(int ChoiceIndex)
{
	if (ChoiceIndex < 0 || ChoiceIndex >= AvailableChoiceIndexes.Num())
	{
		EndDialog();
		return;
	}

	const int RealChoiceIndex = AvailableChoiceIndexes[ChoiceIndex];
	TryGoToChildNode(RealChoiceIndex);
}

void UDialogComponent::Client_DestroyDialogWidget_Implementation()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(OwnerController))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			Pawn->EnableInput(PlayerController);
		}
	}
	
	if (!DisplayedDialogWidget)
	{
		return;
	}

	DisplayedDialogWidget->DisplayLineFinishedDelegate.Unbind();
	DisplayedDialogWidget->DisplayChoicesFinishedDelegate.Unbind();
	DisplayedDialogWidget->RemoveFromParent();
	DisplayedDialogWidget = nullptr;
}

void UDialogComponent::Client_DisplayChoices_Implementation(const TArray<FText>& Choices)
{
	DisplayedDialogWidget->DisplayChoices(Choices);
}

void UDialogComponent::Client_DisplayLine_Implementation(const FText& LineText)
{
	DisplayedDialogWidget->DisplayLine(LineText);
}

void UDialogComponent::Client_CreateDialogWidget_Implementation(
	APlayerController* PlayerController,
	UDialogGraphAsset* DialogAsset
)
{
	// Prevent player moving while dialog is active on client
	if (APawn* Pawn = PlayerController->GetPawn())
	{
		Pawn->DisableInput(PlayerController);
	}
	
	DisplayedDialogWidget = CreateWidget<UDialogWidget>(PlayerController, DialogWidgetClass);
	DisplayedDialogWidget->DisplayLineFinishedDelegate.BindUObject(this, &UDialogComponent::Server_OnLineDisplayed);
	DisplayedDialogWidget->DisplayChoicesFinishedDelegate.BindUObject(this, &UDialogComponent::Server_OnChoiceDisplayed);
	DisplayedDialogWidget->AddToViewport();
	DisplayedDialogWidget->SetCharacterName(DialogAsset->GetCharacterName());
}

void UDialogComponent::Server_OnLineDisplayed_Implementation()
{
	OnLineDisplayedServer();
}

void UDialogComponent::Server_OnChoiceDisplayed_Implementation(int ChoiceIndex)
{
	OnChoicesDisplayedServer(ChoiceIndex);
}
