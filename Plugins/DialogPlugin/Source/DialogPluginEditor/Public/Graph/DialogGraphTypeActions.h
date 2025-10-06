// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class UDialogGraphAsset;
class UDialogEdGraph;

class DIALOGPLUGINEDITOR_API FDialogGraphTypeActions : public FAssetTypeActions_Base
{
public:
	FDialogGraphTypeActions(EAssetTypeCategories::Type InCategoryType);

	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(
		const TArray<UObject*>& InObjects,
		TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()
	) override;
	virtual uint32 GetCategories() override;

private:
	EAssetTypeCategories::Type CategoryType;
};
