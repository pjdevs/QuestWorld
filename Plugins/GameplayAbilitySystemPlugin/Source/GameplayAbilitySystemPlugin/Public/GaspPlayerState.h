// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GrantAbility.h"
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
	: public APlayerState, public IAbilitySystemInterface, public IGrantAbility
{
	GENERATED_BODY()

public:
	AGaspPlayerState();

	virtual void CopyProperties(APlayerState* PlayerState) override;
	
public:
	void GiveGrantedAbilities();
	FGaspPlayerSaveData WriteToSave() const;
	void LoadFromSave(const FGaspPlayerSaveData& SaveData);

	virtual UGaspAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
public: // IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public: // IUnlockAbility interface
	virtual void GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass) override;

protected:
	void GiveAbilityToPlayer(TSubclassOf<UGameplayAbility> AbilityClass);
	
protected:
	UPROPERTY()
	TObjectPtr<UGaspAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UGaspAttributeSet> AttributeSet;

	UPROPERTY()
	TSet<TSubclassOf<UGameplayAbility>> GrantedAbilities;
};
