#include "UtilityTreeWizard.h"

// Window
#include "Interfaces/IMainFrameModule.h"
#include "Framework/Application/SlateApplication.h"

// Slate UI
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSpacer.h"

#include "BehaviorTreeGraph.h"
#include "BehaviorTreeGraphNode.h"
#include "BehaviorTreeGraphNode_Composite.h"
#include "EdGraph/EdGraph.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "EdGraphSchema_BehaviorTree.h"

// Data
#include "SUtilityAction.h"
#include "SWizardSidebar.h"

// Assets
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Engine/Classes/Curves/CurveFloat.h"
#include "UnrealEd/Classes/Factories/CurveFactory.h"
#include "AssetRegistryModule.h"

#include "BTComposite_UtilitySelector.h"
#include "BTComposite_UtilityNode.h"

#include "UtilityAiEditor.h"
#include "SActionListing.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_UtilityAiWizard"

#define UTILITYAI_UPDATEDUE

void OpenWindow(TSharedRef<SWindow> window)
{
	//IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	IMainFrameModule& MainFrameModule = IMainFrameModule::Get();

	if (MainFrameModule.GetParentWindow().IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(window, MainFrameModule.GetParentWindow().ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(window);
	}
}

TSharedRef<SWidget> CreateVerticalBoxFillWidth(TSharedRef<SWidget> child)
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Top)
		.FillWidth(1.0f)
		[child];
}

TSharedRef<SWidget> CreateField(FText label, TSharedRef<SWidget> input)
{
	return SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SNew(STextBlock)
			.Text(label)
		]

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[input]
	;
}

void UtilityTreeWizard::Open()
{
	OpenWindow(
		SAssignNew(mpWindow, SWindow)
		.Title(FText::FromString(TEXT("Create Utility Tree")))
		.ClientSize(FVector2D(600, 800))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		[
			SNew(SScrollBox)
			.Orientation(Orient_Vertical)
			.ScrollBarAlwaysVisible(true)
			.OnUserScrolled(FOnUserScrolled::CreateRaw(this, &UtilityTreeWizard::OnUserScrolled))
			+ SScrollBox::Slot()
			[
				SNew(SVerticalBox)

					// The is the header bar which prompts users to select a behavior tree (preferably the current asset that is open)
					+SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Fill)
					.AutoHeight()
					[ CreateField(
						LOCTEXT("BlackboardAsset_Select", "Select Blackboard Asset"),
						SAssignNew(mpBlackboardSelector, SAssetSelectorBlackboardData)
						.OnAssetSelected(FOnAssetSelected::CreateRaw(this, &UtilityTreeWizard::OnBlackboardAssetSelected))
					) ]

					+SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.FillHeight(1.0f)
					[
						SAssignNew(mpContentPanel, SHorizontalBox)
					]
			]
		]
	);

}

TSharedRef<FUtilityActionDetails> UtilityTreeWizard::GetUnusedAction()
{
	for (auto action : this->mUtilityTreeDetails.mActions)
	{
		if (!action->mShouldGenerate)
			return action;
	}
	auto action = MakeShared<FUtilityActionDetails>();
	action->mIndex = this->mUtilityTreeDetails.mActions.Num();
	action->mShouldGenerate = true;
	return action;
}

void UtilityTreeWizard::OnBlackboardAssetSelected(FAssetData const &asset)
{
	this->mPackageParent = asset.PackagePath;

	UBehaviorTree* treeRaw = Cast<UBehaviorTree>(asset.GetAsset());
	this->mUtilityTreeDetails.mpTree = treeRaw;

	this->mpContentPanel->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.AutoWidth()
		.Padding(20.0f)
	[
		SNew(SWizardSidebar)
		.OnFinish(FSimpleDelegate::CreateRaw(this, &UtilityTreeWizard::GenerateNodes))
		.OnAddAction(FOnAddAction::CreateRaw(this, &UtilityTreeWizard::OnAddAction))
		.OnEditAction(FOnEditAction::CreateRaw(this, &UtilityTreeWizard::OnEditAction))
		.GetUnusedAction(FGetUnusedActionDetail::CreateRaw(this, &UtilityTreeWizard::GetUnusedAction))
	];

	this->mpContentPanel->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillWidth(2.0f)
	[
		SAssignNew(mpWidgetSwitcher, SWidgetSwitcher)
	];

	this->mpWidgetSwitcher->SetActiveWidgetIndex(INDEX_NONE);
}

void UtilityTreeWizard::OnAddAction(TSharedRef<FUtilityActionDetails> action)
{
	if (action->mShouldGenerate)
	{
		this->mUtilityTreeDetails.mActions.Add(action);

		this->mpWidgetSwitcher->AddSlot()
		[
			SNew(SUtilityAction)
			.BlackboardAsset(this->mUtilityTreeDetails.mpTree->BlackboardAsset)
			.Value(TWeakPtr<FUtilityActionDetails>(action))
			.OnDelete(FSimpleDelegate::CreateRaw(this, &UtilityTreeWizard::SetActiveActionToFirstGenerating))
		];
	}
	else
	{
		action->mShouldGenerate = true;
		action->mOnChanged.ExecuteIfBound(TWeakPtr<FUtilityActionDetails>(action));
	}

	this->OnEditAction(action->mIndex);
}

void UtilityTreeWizard::OnEditAction(int32 index)
{
	this->mpWidgetSwitcher->SetActiveWidgetIndex(index);
}

void UtilityTreeWizard::SetActiveActionToFirstGenerating()
{
	for (auto action : this->mUtilityTreeDetails.mActions)
	{
		if (action->mShouldGenerate)
		{
			this->mpWidgetSwitcher->SetActiveWidgetIndex(action->mIndex);
			return;
		}
	}
	this->mpWidgetSwitcher->SetActiveWidgetIndex(INDEX_NONE);
}

// TAKEN FROM: SCurveEditor
UObject* CreateCurveObject(TSubclassOf<UCurveBase> CurveType, UObject* PackagePtr, FName& AssetName)
{
	UObject* NewObj = NULL;
	auto CurveFactory = Cast<UCurveFactory>(NewObject<UFactory>(GetTransientPackage(), UCurveFactory::StaticClass()));
	if (CurveFactory)
	{
		CurveFactory->CurveClass = CurveType;
		NewObj = CurveFactory->FactoryCreateNew(CurveFactory->GetSupportedClass(), PackagePtr, AssetName, RF_Public | RF_Standalone, NULL, GWarn);
	}
	CurveFactory = NULL;
	return NewObj;
}

void UtilityTreeWizard::GenerateNodes()
{
	if (this->mpWindow.IsValid())
	{
		this->mpWindow->RequestDestroyWindow();
		this->mpWindow = nullptr;
	}
	else return;

	FUtilityTreeDetails const &treeDetails = this->mUtilityTreeDetails;

	auto const actions = treeDetails.mActions;

	if (this->mUtilityTreeDetails.mpTree == nullptr || this->mUtilityTreeDetails.mpTree->BlackboardAsset == nullptr)
	{
		return;
	}
	
	auto graphEd = this->mUtilityTreeDetails.mpTree->BTGraph;

#ifdef UTILITYAI_UPDATEDUE
	UBehaviorTreeGraphNode_Composite* graphNode_selector;
	// Create the root selector node
	{
		FGraphNodeCreator<UBehaviorTreeGraphNode_Composite> NodeBuilder(*graphEd);
		graphNode_selector = NodeBuilder.CreateNode();

		graphNode_selector->NodePosX = 300;
		graphNode_selector->NodePosY = 100;
		graphNode_selector->ClassData = FGraphNodeClassData(
			UBTComposite_UtilitySelector::StaticClass(), FString("No Message"));

		NodeBuilder.Finalize();
	}
#endif

	UE_LOG(LogUtilityAiEditor, Log, TEXT("Creating utility tree in BT %s with blackboard %s"),
		*this->mUtilityTreeDetails.mpTree->GetName(),
		*this->mUtilityTreeDetails.mpTree->BlackboardAsset->GetName());

	//auto assetPackage = this->mUtilityTreeDetails.mpTree->GetOutermost();

	TArray<UBehaviorTreeGraphNode_Composite*> utilityActionNodes;
	int i = 0;
	for (auto const action : actions)
	{
		if (!action->mShouldGenerate) continue;

		i++;

		FName const actionName = action->mName;
		UE_LOG(LogUtilityAiEditor, Log, TEXT("Found action: %s"), *actionName.ToString());

#ifdef UTILITYAI_UPDATEDUE
		// Create the graph node
		FGraphNodeCreator<UBehaviorTreeGraphNode_Composite> NodeBuilder(*graphEd);
		auto graphNode_composite = NodeBuilder.CreateNode();
		graphNode_composite->NodePosX = 300 + (i - actions.Num() / 2) * 150;
		graphNode_composite->NodePosY = 300;
		
		// Create the utility action node instance
		graphNode_composite->ClassData = FGraphNodeClassData(
			UBTComposite_UtilityNode::StaticClass(), FString("No Message"));
		
		// Update graph
		NodeBuilder.Finalize();

		auto utilityNode_action = Cast<UBTComposite_UtilityNode>(graphNode_composite->NodeInstance);

		utilityNode_action->NodeName = actionName.ToString();
		// Create the array of inputs for the action
		utilityNode_action->Inputs.Empty();
#endif

		// Iterate through all inputs and add them to the node
		for (auto const actionInputPair : action->mInputs)
		{
			auto const actionInputValue = actionInputPair.Value;
			UE_LOG(LogUtilityAiEditor, Log, TEXT("%s|%s"), *actionName.ToString(),
				*actionInputValue.mBlackboardKeyEntry.mName.ToString()
			);

			// Create the input that will be saved into the node
			auto actionInput = FUtilityInput();
			// Create the blackboard key selector
			actionInput.Key = FBlackboardKeySelector();
			actionInput.Key.SelectedKeyName = actionInputValue.mBlackboardKeyEntry.mName;

			// Create the curve asset for the values in this input
			FName curveAssetFileName = FName(*(
				this->mUtilityTreeDetails.mpTree->BlackboardAsset->GetName()
				+ TEXT("_") + actionName.ToString()
				+ TEXT("_") + actionInputValue.mBlackboardKeyEntry.mName.ToString()
			));
			const FString packageName = mPackageParent.ToString() + TEXT("/") + curveAssetFileName.ToString();
			UPackage* assetPackage = CreatePackage(nullptr, *packageName);
			
			auto curveAsset = Cast<UCurveFloat>(CreateCurveObject(UCurveFloat::StaticClass(), assetPackage, curveAssetFileName));
			if (!curveAsset)
			{
				UE_LOG(LogUtilityAiEditor, Log, TEXT("Could not generate curve asset %s for action %s."), *curveAssetFileName.ToString(), *actionName.ToString());
				continue;
			}

			// Load points into the asset
			auto curvePointMap = actionInputValue.mCurveKeys;

			TArray<FUtilityActionInputCurveKey> curveKeys;
			curvePointMap.GenerateValueArray(curveKeys);
			for (auto const curveKey : curveKeys)
			{
				auto handle = curveAsset->FloatCurve.AddKey(curveKey.input, curveKey.output);
				curveAsset->FloatCurve.Keys[curveAsset->FloatCurve.GetIndexSafe(handle)].InterpMode = ERichCurveInterpMode::RCIM_Cubic;
			}

			// Notify the asset registry
			FAssetRegistryModule::AssetCreated(curveAsset);

			// Mark the package dirty...
			assetPackage->MarkPackageDirty();

			// Link the asset into the node
			actionInput.Curve = curveAsset;

#ifdef UTILITYAI_UPDATEDUE
			// Save the blackboard key / curve asset pair into the node
			utilityNode_action->Inputs.Add(actionInput);
#endif
		}

#ifdef UTILITYAI_UPDATEDUE
		
		// Link utility action node with its parent (the selector node)
		//graphNode_selector->AddSubNode(graphNode_composite, graphEd);
		graphNode_selector->GetOutputPin()->MakeLinkTo(graphNode_composite->GetInputPin());
#endif
	}
	
	this->mUtilityTreeDetails.mActions.Empty();
}

void UtilityTreeWizard::OnUserScrolled(float amount)
{

}

#undef LOCTEXT_NAMESPACE

#ifdef UTILITYAI_UPDATEDUE
#undef UTILITYAI_UPDATEDUE
#endif
