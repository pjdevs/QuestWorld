// Copyright pjdevs. All Rights Reserved.

#include "Graph/DialogGraphAssetFactory.h"
#include "DialogGraphAsset.h"

UDialogGraphAssetFactory::UDialogGraphAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UDialogGraphAsset::StaticClass();
}

UObject* UDialogGraphAssetFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn
)
{
	UDialogGraphAsset* Asset = NewObject<UDialogGraphAsset>(InParent, InName, Flags);
	return Asset;
}

bool UDialogGraphAssetFactory::CanCreateNew() const
{
	return true;
}
