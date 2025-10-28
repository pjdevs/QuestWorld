// Copyright pjdevs. All Rights Reserved.


#include "Dialog/DialogActor.h"
#include "DialogComponent.h"


ADialogActor::ADialogActor()
	: DialogAsset(nullptr), InDialogReasonText(FText::FromString("Talking..."))
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADialogActor::OnStartInteractionInput_Implementation(AActor* InteractionInstigator)
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
	BaseInteractionStatus.ReasonText = InDialogReasonText; // display talking reason (only one possible for now)

	return BaseInteractionStatus;
}

void ADialogActor::OnDialogStarted(AController* DialogController)
{
	StartInteractionPhase(DialogController->GetPawn());
}

void ADialogActor::OnDialogEnded(AController* DialogController)
{
	EndInteractionPhase(EIPInteractiveState::Ready);
}
