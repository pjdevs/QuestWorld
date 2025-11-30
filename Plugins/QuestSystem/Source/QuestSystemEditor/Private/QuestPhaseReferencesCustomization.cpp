// Copyright pjdevs. All Rights Reserved.


#include "QuestPhaseReferencesCustomization.h"

#include "DetailWidgetRow.h"
#include "Assets/QuestDataAsset.h"
#include "Assets/QuestPhaseReferences.h"


TSharedRef<IPropertyTypeCustomization> FQuestPhaseReferencesCustomization::MakeInstance()
{
    return MakeShareable(new FQuestPhaseReferencesCustomization());
}

void FQuestPhaseReferencesCustomization::CustomizeHeader(
    TSharedRef<IPropertyHandle> StructHandle,
    FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
    ThisStructHandle = StructHandle;
    PhasesHandle = ThisStructHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FQuestPhaseReferences, Phases));

    HeaderRow
        .NameContent()
        [
            SNew(STextBlock)
                .Text(FText::FromString("Phases"))
                .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
        ]
        .ValueContent()
        [
            SNew(SComboButton)
                .OnGetMenuContent(this, &FQuestPhaseReferencesCustomization::BuildMenu)
                .ButtonContent()
                [
                    SNew(STextBlock)
                        .Text(this, &FQuestPhaseReferencesCustomization::GetSummary)
                        .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
                ]
        ];
}

void FQuestPhaseReferencesCustomization::CustomizeChildren(
    TSharedRef<IPropertyHandle> StructHandle,
    IDetailChildrenBuilder& ChildBuilder,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
}

TSharedRef<SWidget> FQuestPhaseReferencesCustomization::BuildMenu()
{
    FMenuBuilder Menu(true, nullptr);

    UQuestDataAsset* OwnerQuestAsset = GetOwnerQuestAsset();
    
    if (!OwnerQuestAsset)
    {
        return Menu.MakeWidget();
    }

    for (const FName& Phase : OwnerQuestAsset->Phases)
    {
        const FQuestPhaseReferences* CurrentPhases = GetCurrentPhases();
        TAttribute<bool> bIsPhaseSelected = TAttribute<bool>::Create(
            [CurrentPhases, Phase] { return CurrentPhases->Phases.Contains(Phase); }
        );
        
        FUIAction Action(
            FExecuteAction::CreateSP(this, &FQuestPhaseReferencesCustomization::TogglePhase, Phase),
            FCanExecuteAction(),
            FIsActionChecked::CreateLambda([bIsPhaseSelected] { return bIsPhaseSelected.Get(); })
        );
        
        Menu.AddMenuEntry(
            FText::FromName(Phase),
            FText::GetEmpty(),
            FSlateIcon(),
            Action,
            NAME_None,
            EUserInterfaceActionType::ToggleButton
        );
    }

    return Menu.MakeWidget();
}

void FQuestPhaseReferencesCustomization::TogglePhase(FName Phase) const
{
    FQuestPhaseReferences* CurrentPhases = GetCurrentPhases();

    PhasesHandle->NotifyPreChange();
    
    if (CurrentPhases->Phases.Contains(Phase))
    {
        CurrentPhases->Phases.Remove(Phase);
    }
    else
    {
        CurrentPhases->Phases.Add(Phase);
    }

    PhasesHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
    PhasesHandle->NotifyFinishedChangingProperties();
}

FText FQuestPhaseReferencesCustomization::GetSummary() const
{
    FQuestPhaseReferences* CurrentPhases = GetCurrentPhases();

    if (CurrentPhases->Phases.IsEmpty())
    {
        return FText::FromString("None");
    }
    
    return FText::FromString(CurrentPhases->ToString());
}

FQuestPhaseReferences* FQuestPhaseReferencesCustomization::GetCurrentPhases() const
{
    void* CurrentPtr;
    ThisStructHandle->GetValueData(CurrentPtr);
    return static_cast<FQuestPhaseReferences*>(CurrentPtr);
}

UQuestDataAsset* FQuestPhaseReferencesCustomization::GetOwnerQuestAsset() const
{
    TArray<UObject*> Outers;
    ThisStructHandle->GetOuterObjects(Outers);

    if (Outers.Num() > 0)
    {
        if (const IQuestOwnedObjectInterface* QuestOwnedObject = Cast<IQuestOwnedObjectInterface>(Outers[0]))
        {
            return QuestOwnedObject->GetOwningQuest();
        }
    }

    return nullptr;
}
