#pragma once

#include "CoreMinimal.h"

#include "SAssetSelectorBlackboardData.h"
#include "Widgets/SWindow.h"
#include "UtilityActionDetails.h"

#include "Widgets/Layout/SWidgetSwitcher.h"

struct FUtilityTreeDetails
{
	UBehaviorTree* mpTree;
	UBlackboardData* mpBlackboard;
	TArray<TSharedRef<FUtilityActionDetails>> mActions;
};

class UtilityTreeWizard
{
private:
	TSharedPtr<SAssetSelectorBlackboardData> mpBlackboardSelector;

	TSharedPtr<SWindow> mpWindow;
	TSharedPtr<SHorizontalBox> mpContentPanel;
	TSharedPtr<SWidget> mpButtonFinish, mpButtonAddAction;
	TSharedPtr<SWidgetSwitcher> mpWidgetSwitcher;

	FUtilityTreeDetails mUtilityTreeDetails;

	void OnAddAction(TSharedRef<FUtilityActionDetails> action);
	void OnEditAction(int32 index);

public:
	UtilityTreeWizard() {}

	void Open();
	
	void OnUserScrolled(float amount);

	void OnBlackboardAssetSelected(FAssetData const &asset);
	void GenerateNodes();
	
};
