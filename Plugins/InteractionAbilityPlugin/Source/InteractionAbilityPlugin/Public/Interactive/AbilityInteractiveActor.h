// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "IPInteractiveActor.h"
#include "AbilityInteractiveActor.generated.h"

class UGameplayAbility_Interact;

// TODO Fix this later if really needed but with new start end interaction this would be long to modify
UCLASS()
class INTERACTIONABILITYPLUGIN_API AAbilityInteractiveActor : public AIPInteractiveActor
{
	GENERATED_BODY()

public:
	AAbilityInteractiveActor();

	/**
	 * Will be called on server inside the ability only to ensure that interact has been called by a given ability.
	 * However, this may not avoid calling this on server outside the ability. 
	 */
	void OnPreAbilityInteract(const FGameplayAbilitySpecHandle& AbilitySpecHandle);

	/**
	 * Will be called on server inside the ability after interaction. 
	 */
	void OnPostAbilityInteract();

	/**
	 * Get required ability class to be interacted
	 */
	TSubclassOf<UGameplayAbility_Interact> GetRequiredAbilityClass() const { return RequiredAbilityClass;  }
	
protected: // Interactive
	// virtual void Interact(AActor* InteractionInstigator) override;
	virtual FIPInteractionStatus GetInteractionStatus(AActor* InteractionInstigator) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	FText DoesNotHasRequiredAbilityTextFormat;

	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	FText CannotActivateRequiredAbilityTextFormat;

	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayAbility_Interact> RequiredAbilityClass;

private:
	FGameplayAbilitySpecHandle CurrentAbilitySpecHandle;
};
