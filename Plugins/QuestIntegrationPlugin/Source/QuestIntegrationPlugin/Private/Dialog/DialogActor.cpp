// Copyright pjdevs. All Rights Reserved.


#include "Dialog/DialogActor.h"

#include "DialogComponent.h"
#include "Net/UnrealNetwork.h"


ADialogActor::ADialogActor()
	: DialogAsset(nullptr), InDialogReasonText(FText::FromString("Talking...")), bIsInDialog(false)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADialogActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADialogActor, bIsInDialog);
}

void ADialogActor::DoInteraction_Implementation(AActor* InteractionInstigator)
{
	if (!DialogAsset)
	{
		return;
	}

	if (const APawn* InteractorPawn = Cast<APawn>(InteractionInstigator))
	{
		if (const APlayerController* Controller = Cast<APlayerController>(InteractorPawn->GetController()))
		{
			auto* DialogComponent = Cast<UDialogComponent>(
				Controller->GetComponentByClass(UDialogComponent::StaticClass())
			);

			if (DialogComponent)
			{
				DialogComponent->StartDialog(this, DialogAsset);
			}
		}
	}
}

FIPInteractionStatus ADialogActor::GetInteractionStatus(AActor* InteractionInstigator) const
{
	FIPInteractionStatus BaseInteractionStatus = Super::GetInteractionStatus(InteractionInstigator);

	if (!BaseInteractionStatus.bCanBeInteracted)
	{
		return BaseInteractionStatus;
	}

	// TODO Localize by exposing this default text in properties or se
	return bIsInDialog
		? FIPInteractionStatus { .bCanBeInteracted = false, .ReasonText = InDialogReasonText }
		: FIPInteractionStatus { .bCanBeInteracted = true };
}

void ADialogActor::OnDialogStarted(AController* DialogController)
{
	bIsInDialog = true;
	OnRep_bIsInDialog();
}

void ADialogActor::OnDialogEnded(AController* DialogController)
{
	bIsInDialog = false;
	OnRep_bIsInDialog();
}

void ADialogActor::OnRep_bIsInDialog()
{
	StateChanged();
}
