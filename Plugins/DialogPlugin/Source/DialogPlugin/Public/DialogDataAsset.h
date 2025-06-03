// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogDataAsset.generated.h"

class UDialogNode;

/**
 * 
 */
UCLASS()
class DIALOGPLUGIN_API UDialogDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Dialog, meta = (AllowPrivateAccess = true))
	FText CharacterName;
	
	UPROPERTY(EditDefaultsOnly, Instanced, Category = Dialog, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDialogNode> DialogRoot;

public:
	const FText& GetCharacterName() { return CharacterName; }
	UDialogNode* GetDialogRoot() { return DialogRoot; }
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
