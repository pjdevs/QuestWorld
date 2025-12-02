// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FlowEditor/Public/Asset/AssetDefinition_FlowAsset.h"
#include "QuestFlowAsset_AssetDefinition.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEMEDITOR_API UQuestFlowAsset_AssetDefinition : public UAssetDefinition_FlowAsset
{
	GENERATED_BODY()

public:
	virtual FText GetAssetDisplayName() const override;
	virtual FLinearColor GetAssetColor() const override;
	virtual TSoftClassPtr<UObject> GetAssetClass() const override;
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override;
	virtual FAssetSupportResponse CanLocalize(const FAssetData& InAsset) const override;

	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override;
	virtual EAssetCommandResult PerformAssetDiff(const FAssetDiffArgs& DiffArgs) const override;
};
