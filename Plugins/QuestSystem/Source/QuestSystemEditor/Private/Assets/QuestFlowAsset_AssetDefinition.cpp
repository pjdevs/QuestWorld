// Copyright pjdevs. All Rights Reserved.


#include "Assets/QuestFlowAsset_AssetDefinition.h"

#include "Flow/QuestFlowAsset.h"

FText UQuestFlowAsset_AssetDefinition::GetAssetDisplayName() const
{
	return FText::FromString("Quest Flow Asset");
}

FLinearColor UQuestFlowAsset_AssetDefinition::GetAssetColor() const
{
	return FColor(255, 100, 55);
}

TSoftClassPtr<UObject> UQuestFlowAsset_AssetDefinition::GetAssetClass() const
{
	return UQuestFlowAsset::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UQuestFlowAsset_AssetDefinition::GetAssetCategories() const
{
	static const auto Categories = {
		FAssetCategoryPath(FText::FromName("Quest")),
	};
	 
	return Categories;
}

FAssetSupportResponse UQuestFlowAsset_AssetDefinition::CanLocalize(const FAssetData& InAsset) const
{
	return FAssetSupportResponse::Supported();
}

EAssetCommandResult UQuestFlowAsset_AssetDefinition::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	// for (UQuestFlowAsset* FlowAsset : OpenArgs.LoadObjects<UQuestFlowAsset>())
	// {
	// 	const FFlowEditorModule* FlowModule = &FModuleManager::LoadModuleChecked<FFlowEditorModule>("FlowEditor");
	// 	FlowModule->CreateFlowAssetEditor(OpenArgs.GetToolkitMode(), OpenArgs.ToolkitHost, FlowAsset);
	// }
	//
	// return EAssetCommandResult::Handled;
	return Super::OpenAssets(OpenArgs);
}

EAssetCommandResult UQuestFlowAsset_AssetDefinition::PerformAssetDiff(const FAssetDiffArgs& DiffArgs) const
{
	return Super::PerformAssetDiff(DiffArgs);
}
