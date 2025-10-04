// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "DialogGraphAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGPLUGINEDITOR_API UDialogGraphAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UDialogGraphAssetFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(
		UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		UObject* Context,
		FFeedbackContext* Warn
	) override;
	virtual bool CanCreateNew() const override;
};
