// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractionStatus.h"
#include "UObject/Interface.h"
#include "IPInteractive.generated.h"

class UIPInteractionWidget;
class UWidgetComponent;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UIPInteractive : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for interactive objects. Can only be implemented by actors.
 */
class INTERACTIONPLUGIN_API IIPInteractive
{
	GENERATED_BODY()

public:
	/**
	 * Begin interaction input with the interactive object.
	 */
	virtual void StartInteractionInput(AActor* InteractionInstigator) = 0;

	/**
	 * End interaction input with the interactive object.
	 */
	virtual void EndInteractionInput(AActor* InteractionInstigator) = 0;

	/**
	 * Check whether the object can be interacted or not by giving information on interaction status. 
	 */
	virtual FIPInteractionStatus GetInteractionStatus(AActor* InteractionInstigator) const = 0;

	/**
	 * Get interactive's location. 
	 */
	virtual FVector GetInteractiveLocation() const = 0;

	/**
	 * Get the in world widget component to display widgets inside. 
	 */
	virtual UWidgetComponent* GetWidgetComponent() const = 0;

	/**
	 * The class of interaction widget to use. 
	 */
	virtual TSubclassOf<UIPInteractionWidget> GetInteractionWidgetClass() const = 0;

	/**
	 * The class if indication widget to use. 
	 */
	virtual TSubclassOf<UUserWidget> GetIndicationWidgetClass() const = 0;

	/**
	 * The class if indication widget to use when interaction is not possible. 
	 */
	virtual TSubclassOf<UUserWidget> GetIndicationBlockedWidgetClass() const = 0;

	/**
	 * Get the description of the interaction the show in the interaction widget.
	 */
	virtual FText GetInteractiveName() const = 0;
	
	/**
	 * Get the description of the interaction the show in the interaction widget.
	 */
	virtual FText GetInteractionDescription() const = 0;

	/**
	 * Whether this interactive is auto interactive.
	 */
	virtual bool IsAutoInteractive() const = 0;
};
