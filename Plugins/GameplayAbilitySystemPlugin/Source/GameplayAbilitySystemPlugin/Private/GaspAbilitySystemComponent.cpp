// Copyright pjdevs. All Rights Reserved.


#include "GaspAbilitySystemComponent.h"


UGaspAbilitySystemComponent::UGaspAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UGaspAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}
 