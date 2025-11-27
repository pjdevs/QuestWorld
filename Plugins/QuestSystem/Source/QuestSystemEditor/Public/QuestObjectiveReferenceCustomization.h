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
	TSharedRef<SWidget> BuildMenu();
	void SetObjectiveId(FName ObjectiveIdName) const;
	FText GetSummary() const;
	
private:
	TSharedPtr<IPropertyHandle> QuestRefHandle;
	TSharedPtr<IPropertyHandle> ObjectiveIdHandle;
};