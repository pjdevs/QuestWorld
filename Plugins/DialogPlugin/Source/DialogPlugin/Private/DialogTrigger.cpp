#include "DialogTrigger.h"

UWorld* UDialogTrigger::GetWorld() const
{
	return World;
}

void UDialogTrigger::Execute(UWorld* InWorld, AController* DialogController)
{
	World = InWorld;

	OnExecute(DialogController);
	
	World = nullptr;
}

void UDialogTrigger::OnExecute_Implementation(AController* DialogController)
{
}
