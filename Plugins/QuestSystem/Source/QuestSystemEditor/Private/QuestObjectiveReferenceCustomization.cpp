// Copyright pjdevs. All Rights Reserved.


#include "QuestObjectiveReferenceCustomization.h"

#include "Assets/QuestDataAsset.h"
#include "Assets/QuestObjectiveReference.h"
#include "DetailWidgetRow.h"


template<typename T>
T* GetHandleValue(TSharedPtr<IPropertyHandle> Handle)
{
    void* CurrentPtr;
    Handle->GetValueData(CurrentPtr);
    return static_cast<T*>(CurrentPtr);
}

template<typename T>
void SetHandleValue(TSharedPtr<IPropertyHandle> Handle, const T& Value)
{
    T* ValuePtr = GetHandleValue<T>(Handle);
    *ValuePtr = Value;
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

    TSoftObjectPtr<UQuestDataAsset> QuestRef = nullptr;
    
    UObject* Owner = nullptr;
    TArray<UObject*> OuterObjects;
    StructHandle->GetOuterObjects(OuterObjects);
    if (OuterObjects.Num() > 0)
    {
        Owner = OuterObjects[0];
    }

    FString GetQuestRefFuncName = StructHandle->GetProperty()->GetMetaData("QuestReference");
    if (!GetQuestRefFuncName.IsEmpty() && Owner)
    {
        if (UFunction* GetQuestRefFunc = Owner->FindFunction(FName(*GetQuestRefFuncName)))
        {
            struct FReturn { TSoftObjectPtr<UQuestDataAsset> QuestRef; } ReturnValue;
            Owner->ProcessEvent(GetQuestRefFunc, &ReturnValue);
            QuestRef = ReturnValue.QuestRef;
        }
    }

    FQuestObjectiveReference* ObjectiveRef = GetHandleValue<FQuestObjectiveReference>(StructHandle);

    if (!ObjectiveRef->IsValid() || ObjectiveRef->QuestRef != QuestRef)
    {
        StructHandle->NotifyPreChange();
        SetHandleValue(QuestRefHandle, QuestRef);
        ObjectiveRef->FixupReference();
        StructHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
        StructHandle->NotifyFinishedChangingProperties();
    }

    ObjectiveIds.Empty();

    if (const UQuestDataAsset* QuestAsset = QuestRef.LoadSynchronous())
    {
        for (const UQuestObjective* Objective : QuestAsset->Objectives)
        {
            ObjectiveIds.Add(Objective->ObjectiveId);   
        }
    }
    
    HeaderRow
        .NameContent()[StructHandle->CreatePropertyNameWidget()]
        .ValueContent()
        [
            SAssignNew(ComboBox, SComboBox<FName>)
                .OptionsSource(&ObjectiveIds)
                .Content()
                [
                    SNew(STextBlock)
                        .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
                        .Text(this, &FQuestObjectiveReferenceCustomization::GetContentText)
                ]
                .OnGenerateWidget_Raw(this, &FQuestObjectiveReferenceCustomization::GenerateWidget)
                .OnSelectionChanged_Raw(this, &FQuestObjectiveReferenceCustomization::OnSelectionChanged)
                .InitiallySelectedItem(ObjectiveRef->ObjectiveId)
        ];
}

void FQuestObjectiveReferenceCustomization::CustomizeChildren(
    TSharedRef<IPropertyHandle> StructHandle,
    IDetailChildrenBuilder& ChildBuilder,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
}

FText FQuestObjectiveReferenceCustomization::GetContentText() const
{
    return FText::FromName(ComboBox->GetSelectedItem());
}

TSharedRef<SWidget> FQuestObjectiveReferenceCustomization::GenerateWidget(FName Name) const
{
    return SNew(STextBlock)
        .Text(FText::FromName(Name))
        .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"));
}

void FQuestObjectiveReferenceCustomization::OnSelectionChanged(FName SelectedObjectiveId, ESelectInfo::Type SelectionType) const
{
    ObjectiveIdHandle->NotifyPreChange();
    SetHandleValue(ObjectiveIdHandle, SelectedObjectiveId);
    ObjectiveIdHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
    ObjectiveIdHandle->NotifyFinishedChangingProperties();
}
