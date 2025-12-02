// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UQuestDataAsset;

/**
 * Customization to allow only parent quest phase selection.
 */
class FQuestObjectiveReferenceCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

private:
	virtual void CustomizeHeader(
		TSharedRef<IPropertyHandle> StructHandle,
		FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils
	) override;

	virtual void CustomizeChildren(
	TSharedRef<IPropertyHandle> StructHandle,
		IDetailChildrenBuilder& ChildBuilder,
		IPropertyTypeCustomizationUtils& CustomizationUtils
	) override;

private:
	FText GetContentText() const;
	TSharedRef<SWidget> GenerateWidget(FName Name) const;
	void OnSelectionChanged(FName SelectedObjectiveId, ESelectInfo::Type SelectionType) const;

private:
	TSharedPtr<IPropertyHandle> QuestRefHandle;
	TSharedPtr<IPropertyHandle> ObjectiveIdHandle;

	TArray<FName> ObjectiveIds;

	TSharedPtr<SComboBox<FName>> ComboBox;
};