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

	UPROPERTY(EditDefaultsOnly, Instanced, Category = Dialog, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDialogNode> DialogRoot;

public:
	UDialogNode* GetDialogRoot() { return DialogRoot; }
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
