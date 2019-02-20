#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardData.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

struct FUtilityActionDetails
{
	FName mName;
};

struct FUtilityTreeDetails
{
	UBlackboardData *mpBlackboard;
	TArray<FUtilityActionDetails> mActions;
};

class SelectorBlackboardData
{
	UBlackboardData *mpAsset;

	TArray<const UClass*> mClasses;
	TSharedPtr<STextBlock> mpWidgetAssetName;

public:
	void GetAllowedClasses(TArray<const UClass*>& classes);
	void OnSelectedAsset(const FAssetData& AssetData);
	void CloseMenuAsset();

	TSharedRef<SWidget> Construct();

};

class UtilityTreeWizard
{
private:
	SelectorBlackboardData mpBlackboardSelector[1];

	TSharedPtr<SWidgetSwitcher> mpWidgetSwitcher;

	FUtilityTreeDetails mUtilityTreeDetails;

public:
	UtilityTreeWizard() {}

	void Open();
	
	void OnUserScrolled(float amount);

	TSharedRef<SWidget> BuildMenuSelectBlackboardAsset();

	//TArray<TSharedPtr< FComboTest>> Options;
	//TSharedPtr<FComboTest> CurrentItem;
	//TSharedPtr<STextBlock> CurrentText;
	//CurrentItem = MakeShareable(new FComboTest{ LOCTEXT("CreateNew", "New Blackboard Key") });
	//Options.Add(CurrentItem);
	//Options.Add(MakeShareable(new FComboTest{ LOCTEXT("A", "A") }));
	/*
	void FUtilityAiEditorModule::UpdateProperty()
	{
		//if (CurrentItem == Options.Last())
		{
			// Show custom loop entry
			//LoopEntry->SetVisibility(EVisibility::Visible);
		}
		//else
		{
			// Hide custom loop entry
			//LoopEntry->SetVisibility(EVisibility::Collapsed);
		}

		//LoopCountProperty->SetValue(CurrentItem->Value);
		//CurrentText->SetText(CurrentItem->Label);
	}
	*/

};
