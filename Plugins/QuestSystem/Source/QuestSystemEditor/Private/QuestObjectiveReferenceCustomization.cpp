// Copyright pjdevs. All Rights Reserved.


#include "QuestObjectiveReferenceCustomization.h"

#include "Assets/QuestDataAsset.h"
#include "Assets/QuestObjectiveReference.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"


template<typename T>
T* GetHandleValue(TSharedPtr<IPropertyHandle> Handle)
{
    void* CurrentPtr;
    Handle->GetValueData(CurrentPtr);
    return static_cast<T*>(CurrentPtr);
}

TSharedRef<IPropertyTypeCustomization> FQuestObjectiveReferenceCustomization::MakeInstance()
{
    return MakeShareable(new FQuestObjectiveReferenceCustomization());
}

void FQuestObjectiveReferenceCustomization::CustomizeHeader(
    TSharedRef<IPropertyHandle> StructHandle,
    FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
    QuestRefHandle = StructHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FQuestObjectiveReference, QuestRef));
    ObjectiveIdHandle = StructHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FQuestObjectiveReference, ObjectiveId));

    HeaderRow.NameContent()[StructHandle->CreatePropertyNameWidget()];
}

void FQuestObjectiveReferenceCustomization::CustomizeChildren(
    TSharedRef<IPropertyHandle> StructHandle,
    IDetailChildrenBuilder& ChildBuilder,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
    ChildBuilder.AddProperty(QuestRefHandle.ToSharedRef());
    
    ChildBuilder
        .AddCustomRow(FText::FromString("Objective"))
        .NameContent()
        [
            SNew(STextBlock)
                .Text(FText::FromString("Objective"))
                .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
        ]
        .ValueContent()
        [
            SNew(SComboButton)
                .OnGetMenuContent(this, &FQuestObjectiveReferenceCustomization::BuildMenu)
                .ButtonContent()
                [
                    SNew(STextBlock)
                        .Text(this, &FQuestObjectiveReferenceCustomization::GetSummary)
                        .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
                ]
        ];
}

TSharedRef<SWidget> FQuestObjectiveReferenceCustomization::BuildMenu()
{
    FMenuBuilder Menu(true, nullptr);

    const TSoftObjectPtr<UQuestDataAsset>* QuestRef = GetHandleValue<TSoftObjectPtr<UQuestDataAsset>>(QuestRefHandle);
    
    if (QuestRef->IsNull())
    {
        return Menu.MakeWidget();
    }

    UQuestDataAsset* QuestAsset = QuestRef->LoadSynchronous();

    if (!QuestAsset)
    {
        return Menu.MakeWidget();
    }

    for (const TObjectPtr<UQuestObjective>& Objective : QuestAsset->Objectives)
    {
        FUIAction Action(
            FExecuteAction::CreateSP(
                this,
                &FQuestObjectiveReferenceCustomization::SetObjectiveId,
                Objective->ObjectiveId
            )
        );
        
        Menu.AddMenuEntry(
            FText::FromName(Objective->ObjectiveId),
            FText::GetEmpty(),
            FSlateIcon(),
            Action
        );
    }

    return Menu.MakeWidget();
}

void FQuestObjectiveReferenceCustomization::SetObjectiveId(FName ObjectiveId) const
{
    ObjectiveIdHandle->NotifyPreChange();
    
    FName* ObjectiveIdPtr = GetHandleValue<FName>(ObjectiveIdHandle);
    *ObjectiveIdPtr = ObjectiveId;

    ObjectiveIdHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
    ObjectiveIdHandle->NotifyFinishedChangingProperties();
}

FText FQuestObjectiveReferenceCustomization::GetSummary() const
{
    const FName* ObjectiveIdPtr = GetHandleValue<FName>(ObjectiveIdHandle);
    return FText::FromName(*ObjectiveIdPtr);
}
