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

FIPInteractionStatus ADialogActor::GetExtraInteractionStatusForActor_Implementation(
	AActor* InteractionInstigator,
	EIPState CurrentState
) const
{
	return FIPInteractionStatus
	{
		.bCanStartInteraction = true,
		.ReasonText = InDialogReasonText
	};
}

void ADialogActor::OnDialogStarted(AController* DialogController)
{
	StartInteractionPhase(DialogController->GetPawn());
}

void ADialogActor::OnDialogEnded(AController* DialogController)
{
	EndInteractionPhase(EIPState::Idle);
}
