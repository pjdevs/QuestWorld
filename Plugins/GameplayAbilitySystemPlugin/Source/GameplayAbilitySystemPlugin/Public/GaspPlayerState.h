// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GrantAbility.h"
#include "ISpudObject.h"
#include "Save/GaspPlayerSaveData.h"
#include "GaspPlayerState.generated.h"

class UGameplayAbility;
class UGaspAttributeSet;
class UGaspAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class GAMEPLAYABILITYSYSTEMPLUGIN_API AGaspPlayerState
	: public APlayerState, public IAbilitySystemInterface, public IGrantAbility, public ISpudObject, public ISpudObjectCallback
{
	GENERATED_BODY()

public:
	AGaspPlayerState();
	
public:
	void GiveGrantedAbilities();

	virtual UGaspAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
public: // IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public: // IUnlockAbility interface
	virtual void GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass) override;

public: // Spud
	virtual void SpudPostRestore_Implementation(const USpudState* State) override;

protected:
	void GiveAbilityToPlayer(TSubclassOf<UGameplayAbility> AbilityClass);
	
protected:
	UPROPERTY()
	TObjectPtr<UGaspAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UGaspAttributeSet> AttributeSet;

	UPROPERTY(SaveGame)
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;
};
