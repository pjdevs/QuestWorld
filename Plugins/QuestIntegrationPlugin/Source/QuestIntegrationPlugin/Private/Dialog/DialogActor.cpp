// Copyright pjdevs. All Rights Reserved.


#include "Dialog/DialogActor.h"

#include "DialogComponent.h"


ADialogActor::ADialogActor()
	: DialogAsset(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADialogActor::DoInteract(AActor* InteractionInstigator)
{
	if (!DialogAsset)
		return;

	UDialogComponent* DialogComponent = Cast<UDialogComponent>(
		InteractionInstigator->GetComponentByClass(UDialogComponent::StaticClass())
	);

	if (DialogComponent)
	{
		DialogComponent->StartDialog(DialogAsset);
	}
}
