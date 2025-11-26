// Copyright pjdevs. All Rights Reserved.


#include "QuestPhaseListCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "Assets/QuestDataAsset.h"
#include "Assets/QuestPhaseList.h"


TSharedRef<IPropertyTypeCustomization> FQuestPhaseListCustomization::MakeInstance()
{
    return MakeShareable(new FQuestPhaseListCustomization());
}

void FQuestPhaseListCustomization::CustomizeHeader(
    TSharedRef<IPropertyHandle> StructHandle,
    FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
    ThisStructHandle = StructHandle;
    PhasesHandle = ThisStructHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FQuestPhaseList, Phases));

    //ChildBuilder.AddCustomRow(FText::FromString("Phases"))
    HeaderRow
    .NameContent()
    [
        SNew(STextBlock).Text(FText::FromString("Phases"))
    ]
    .ValueContent()
    [
        SNew(SComboButton)
        .OnGetMenuContent(this, &FQuestPhaseListCustomization::BuildMenu)
        .ButtonContent()
        [
            SNew(STextBlock).Text(this, &FQuestPhaseListCustomization::GetSummary)
        ]
    ];
    // HeaderRow.NameContent()[StructHandle->CreatePropertyNameWidget()];
}

void FQuestPhaseListCustomization::CustomizeChildren(
    TSharedRef<IPropertyHandle> StructHandle,
    IDetailChildrenBuilder& ChildBuilder,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
}

TSharedRef<SWidget> FQuestPhaseListCustomization::BuildMenu()
{
    FMenuBuilder Menu(true, nullptr);

    UQuestDataAsset* OwnerQuestAsset = GetOwnerQuestAsset();
    
    if (!OwnerQuestAsset)
    {
        return Menu.MakeWidget();
    }

    for (const FGameplayTag& Phase : OwnerQuestAsset->Phases)
    {
        const FGameplayTagContainer* CurrentPhases = GetCurrentPhases();
        TAttribute<bool> bIsPhaseSelected = TAttribute<bool>::Create(
        [CurrentPhases, Phase] { return CurrentPhases->HasTagExact(Phase); }
        );
        
        FUIAction Action(
            FExecuteAction::CreateSP(this, &FQuestPhaseListCustomization::TogglePhase, Phase),
            FCanExecuteAction(),
            FIsActionChecked::CreateLambda([bIsPhaseSelected] { return bIsPhaseSelected.Get(); })
        );
        
        Menu.AddMenuEntry(
            FText::FromName(Phase.GetTagName()),
            FText::GetEmpty(),
            FSlateIcon(),
            Action,
            NAME_None,
            EUserInterfaceActionType::ToggleButton
        );
    }

    return Menu.MakeWidget();
}

void FQuestPhaseListCustomization::TogglePhase(FGameplayTag Phase) const
{
    FGameplayTagContainer* CurrentPhases = GetCurrentPhases();

    PhasesHandle->NotifyPreChange();
    
    if (CurrentPhases->HasTagExact(Phase))
    {
        CurrentPhases->RemoveTag(Phase);
    }
    else
    {
        CurrentPhases->AddTag(Phase);
    }

    PhasesHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
    PhasesHandle->NotifyFinishedChangingProperties();
}

FText FQuestPhaseListCustomization::GetSummary() const
{
    FGameplayTagContainer* CurrentPhases = GetCurrentPhases();

    if (CurrentPhases->IsEmpty())
    {
        return FText::FromString("None");
    }

    return FText::FromString(CurrentPhases->ToStringSimple());
}

FGameplayTagContainer* FQuestPhaseListCustomization::GetCurrentPhases() const
{
    void* CurrentPtr;
    PhasesHandle->GetValueData(CurrentPtr);
    return static_cast<FGameplayTagContainer*>(CurrentPtr);
}

UQuestDataAsset* FQuestPhaseListCustomization::GetOwnerQuestAsset() const
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
