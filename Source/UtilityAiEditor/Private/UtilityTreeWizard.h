#pragma once

#include "CoreMinimal.h"

#include "SAssetSelectorBlackboardData.h"
#include "Widgets/SWindow.h"
#include "UtilityActionDetails.h"

#include "Widgets/Layout/SWidgetSwitcher.h"

struct FUtilityTreeDetails
{
	UBehaviorTree* mpTree;
	TArray<TSharedRef<FUtilityActionDetails>> mActions;
};

class UtilityTreeWizard
{
private:
	TSharedPtr<SAssetSelectorBlackboardData> mpBlackboardSelector;
	FName mPackageParent;

	TSharedPtr<SWindow> mpWindow;
	TSharedPtr<SHorizontalBox> mpContentPanel;
	TSharedPtr<SWidget> mpButtonFinish, mpButtonAddAction;
	TSharedPtr<SWidgetSwitcher> mpWidgetSwitcher;

	FUtilityTreeDetails mUtilityTreeDetails;

	void OnAddAction(TSharedRef<FUtilityActionDetails> action);
	void OnEditAction(int32 index);
	TSharedRef<FUtilityActionDetails> GetUnusedAction();
	void SetActiveActionToFirstGenerating();

public:
	UtilityTreeWizard() {}

	void Open();
	
	void OnUserScrolled(float amount);

	void OnBlackboardAssetSelected(FAssetData const &asset);
	void GenerateNodes();
	
};
