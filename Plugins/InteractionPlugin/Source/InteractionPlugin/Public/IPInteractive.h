// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPInteractive.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UIPInteractive : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for interactive objects.
 */
class INTERACTIONPLUGIN_API IIPInteractive
{
	GENERATED_BODY()

public:
	/**
	 * Interact with the interactive object.
	 */
	virtual void Interact(AActor* InteractionInstigator) = 0;

	/**
	 * Check whether the object can be interacted or not. 
	 */
	virtual bool CanBeInteracted(AActor* InteractionInstigator) = 0;

	/**
	 * Get interactive's location. 
	 */
	virtual FVector GetInteractiveLocation() const = 0;
	
	/*
	 * Get the description of the interaction the show in the interaction widget.
	 */
	virtual FString GetInteractionDescription() const = 0;

	/*
	 * Whether this interactive is auto interactive.
	 */
	virtual bool IsAutoInteractive() const = 0;
};
