#pragma once

#include "CoreMinimal.h"

#include "SAssetSelectorBlackboardData.h"

#include "Widgets/Layout/SWidgetSwitcher.h"


struct FUtilityActionDetails
{
	FName mName;
};

struct FUtilityTreeDetails
{
	TSharedPtr<UBlackboardData> mpBlackboard;
	TArray<FUtilityActionDetails> mActions;
};

class UtilityTreeWizard
{
	static const FText TextButtonFinish;
	static const FText TextButtonAddAction;

private:
	TSharedPtr<SAssetSelectorBlackboardData> mpBlackboardSelector;

	TSharedPtr<SWidget> mpButtonFinish, mpButtonAddAction;
	TSharedPtr<SWidgetSwitcher> mpWidgetSwitcher;

	FUtilityTreeDetails mUtilityTreeDetails;

public:
	UtilityTreeWizard() {}

	void Open();
	
	void OnUserScrolled(float amount);

	void OnBlackboardAssetSelected(FAssetData const &asset);
	void AddAction();
	void GenerateNodes();

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
