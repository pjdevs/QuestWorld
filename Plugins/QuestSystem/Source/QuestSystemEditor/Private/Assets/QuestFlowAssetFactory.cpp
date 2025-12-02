// Copyright pjdevs. All Rights Reserved.


#include "Assets/QuestFlowAssetFactory.h"

#include "Flow/QuestFlowAsset.h"
#include "Graph/FlowGraph.h"

UQuestFlowAssetFactory::UQuestFlowAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UQuestFlowAsset::StaticClass();

	bCreateNew = true;
	bEditorImport = false;
	bEditAfterNew = true;
}

UObject* UQuestFlowAssetFactory::FactoryCreateNew(
	UClass* InClass,
	UObject* InParent,
	FName InName,
	EObjectFlags Flags,
	UObject* Context,
	FFeedbackContext* Warn
)
{
	UQuestFlowAsset* NewFlowAsset = NewObject<UQuestFlowAsset>(
		InParent,
		UQuestFlowAsset::StaticClass(),
		InName,
		Flags | RF_Transactional,
		Context
	);
	
	UFlowGraph::CreateGraph(NewFlowAsset);

	return NewFlowAsset;
}
