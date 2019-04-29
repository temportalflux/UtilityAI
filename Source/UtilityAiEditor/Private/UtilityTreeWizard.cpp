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

//#define UTILITYAI_UPDATEDUE

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
	//this->mUtilityTreeDetails.mpBlackboard.Reset();

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

void UtilityTreeWizard::OnBlackboardAssetSelected(FAssetData const &asset)
{
	UBehaviorTree* treeRaw = Cast<UBehaviorTree>(asset.GetAsset());
	//TSharedPtr<UBehaviorTree> treePtr = MakeShareable(treeRaw);
	//TSharedRef<UBehaviorTree> treeRef(treeRaw);
	//TWeakPtr<UBehaviorTree> treeWeak(treeRef);
	//TWeakPtr<UBehaviorTree> treeWeak(TSharedRef<UBehaviorTree>(treeRaw));
	
	//this->mpBehaviorTreeAsset = treeWeak;
	//this->mpBehaviorTreeAsset = TWeakPtr<UBehaviorTree>(TSharedRef<UBehaviorTree>(treeRaw));

	this->mUtilityTreeDetails.mpTree = treeRaw;

	/*if (!this->mpBehaviorTreeAsset.Pin().IsValid() || this->mpBehaviorTreeAsset.Pin()->BlackboardAsset == nullptr)
	{
		UE_LOG(LogUtilityAiEditor, Log, TEXT("No asset found for %s"), *asset.GetFullName());
		return;
	}*/

	//TSharedRef<UBlackboardData> blackboardRef(this->mpBehaviorTreeAsset.Pin()->BlackboardAsset);
	//TWeakPtr<UBlackboardData> blackboardWeak(blackboardRef);
	//this->mUtilityTreeDetails.mpBlackboard = blackboardWeak;
	//this->mUtilityTreeDetails.mpBlackboard = TWeakPtr<UBlackboardData>(TSharedRef<UBlackboardData>(this->mpBehaviorTreeAsset.Pin()->BlackboardAsset));
	this->mUtilityTreeDetails.mpBlackboard = treeRaw->BlackboardAsset;
	
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
	this->mUtilityTreeDetails.mActions.Add(action);
	
	this->mpWidgetSwitcher->AddSlot()
	[
		SNew(SUtilityAction)
		.BlackboardAsset(this->mUtilityTreeDetails.mpBlackboard)
		.Value(TWeakPtr<FUtilityActionDetails>(action))
	];	

	this->OnEditAction(action->mIndex);
}

void UtilityTreeWizard::OnEditAction(int32 index)
{
	this->mpWidgetSwitcher->SetActiveWidgetIndex(index);
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

	if (this->mUtilityTreeDetails.mpTree == nullptr || this->mUtilityTreeDetails.mpBlackboard == nullptr)
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

		graphNode_selector->NodePosX = 50;
		graphNode_selector->NodePosY = 50;

		auto utilityNode_selector = NewObject<UBTComposite_UtilitySelector>();
		utilityNode_selector->NodeName = TEXT("Utility Action Selector");
		graphNode_selector->NodeInstance = utilityNode_selector;

		NodeBuilder.Finalize();
	}
#endif

	UE_LOG(LogUtilityAiEditor, Log, TEXT("Creating utility tree in BT %s with blackboard %s"),
		*this->mUtilityTreeDetails.mpTree->GetName(),
		*this->mUtilityTreeDetails.mpBlackboard->GetName());

	auto assetPackage = this->mUtilityTreeDetails.mpTree->GetOutermost();

	TArray<UBehaviorTreeGraphNode_Composite*> utilityActionNodes;
	for (auto const action : actions)
	{
		FName const actionName = action->mName;
		UE_LOG(LogUtilityAiEditor, Log, TEXT("Found action: %s"), *actionName.ToString());

#ifdef UTILITYAI_UPDATEDUE
		// Create the graph node
		FGraphNodeCreator<UBehaviorTreeGraphNode_Composite> NodeBuilder(*graphEd);
		auto graphNode_composite = NodeBuilder.CreateNode();
		graphNode_composite->NodePosX = 50;
		graphNode_composite->NodePosY = 50;
		
		// Create the utility action node instance
		auto utilityNode_action = NewObject<UBTComposite_UtilityNode>();
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
			FName curveAssetFileName = FName(*(this->mUtilityTreeDetails.mpBlackboard->GetName() + TEXT("_") + actionName.ToString()));
			auto curveAsset = Cast<UCurveFloat>(CreateCurveObject(UCurveFloat::StaticClass(), assetPackage, curveAssetFileName));
			if (!curveAsset)
			{
				UE_LOG(LogUtilityAiEditor, Log, TEXT("Could not generate curve asset %s for action %s."), *curveAssetFileName.ToString(), *actionName.ToString());
				continue;
			}
			UE_LOG(LogUtilityAiEditor, Log,
				TEXT("Generating curve named %s at %s, next outer is %i"),
				*curveAssetFileName.ToString(),
				*assetPackage->GetFullName(),
				assetPackage->GetOuter() != nullptr
			);
			if (assetPackage->GetOuter())
			UE_LOG(LogUtilityAiEditor, Log,
				TEXT("Generating curve named %s at %s, next outer is %i"),
				*curveAssetFileName.ToString(),
				*assetPackage->GetFullName(),
				*assetPackage->GetOuter()->GetFullName()
			);

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

			// Link the asset into the node
			actionInput.Curve = curveAsset;

#ifdef UTILITYAI_UPDATEDUE
			// Save the blackboard key / curve asset pair into the node
			utilityNode_action->Inputs.Add(actionInput);
#endif
		}

#ifdef UTILITYAI_UPDATEDUE
		// Update graph
		graphNode_composite->NodeInstance = utilityNode_action;
		NodeBuilder.Finalize();
		
		// Link utility action node with its parent (the selector node)
		graphNode_selector->AddSubNode(graphNode_composite, graphEd);
#endif
	}

	// Mark the package dirty...
	assetPackage->MarkPackageDirty();
	
	this->mUtilityTreeDetails.mActions.Empty();
}

void UtilityTreeWizard::OnUserScrolled(float amount)
{

}

#undef LOCTEXT_NAMESPACE

#ifdef UTILITYAI_UPDATEDUE
#undef UTILITYAI_UPDATEDUE
#endif
