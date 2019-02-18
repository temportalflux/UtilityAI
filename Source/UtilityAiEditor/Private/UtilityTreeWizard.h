#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardData.h"

struct FUtilityTreeDetails
{
	UBlackboardData *mpBlackboard;

};

class UtilityTreeWizard
{
private:
	TArray<const UClass*> mBlackboardClasses;

	TSharedPtr<STextBlock> mWidgetBlackboardAssetName;

	FUtilityTreeDetails mUtilityTreeDetails;

public:
	UtilityTreeWizard() {}

	void Open();
	TSharedRef<SWidget> BuildMenuSelectBlackboardAsset();
	void GetAllowedClasses(TArray<const UClass*>& classes);
	void OnSelectedBlackboardAsset(const FAssetData& AssetData);
	void CloseMenuBlackboardAsset();

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
