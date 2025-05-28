// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IPInteractionWidget.generated.h"

/**
 * Widget base class used to show display interaction description on client.
 */
UCLASS(Abstract)
class INTERACTIONPLUGIN_API UIPInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetInteractionDescription(const FString& InteractionDescription);
};
