// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogGraphAsset.generated.h"

class UDialogNode;

/**
 * Asset representing a dialog graph editable in a custom dialog graph editor.
 */
UCLASS()
class DIALOGPLUGIN_API UDialogGraphAsset : public UObject
{
	GENERATED_BODY()

public:
	const FText& GetCharacterName() { return CharacterName; }
	UDialogNode* GetDialogRoot() { return DialogRoot; }
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

private:
	UPROPERTY(EditAnywhere, Category = Dialog, meta = (AllowPrivateAccess = true))
	FText CharacterName;
	
	UPROPERTY(EditAnywhere, Instanced, Category = Dialog, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDialogNode> DialogRoot;
};
