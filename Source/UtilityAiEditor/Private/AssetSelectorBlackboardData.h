#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BlackboardData.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Text/STextBlock.h"

#include "PropertyCustomizationHelpers.h"

class AssetSelectorBlackboardData
{
	static const FText TextSelectionPrompt;
	static const FText TextSelectionConfirm;

	UBlackboardData *mpAsset;
	FOnAssetSelected mpOnAssetConfirmed;

	TArray<const UClass*> mClasses;
	TSharedPtr<STextBlock> mpWidgetAssetName;
	TSharedPtr<SComboButton> mpButtonSelector;
	TSharedPtr<SButton> mpButtonConfirm;

	void GetAllowedClasses(TArray<const UClass*>& classes);
	TSharedRef<SWidget> BuildSelectMenu();
	void OnSelectedAsset(const FAssetData& AssetData);
	void CloseMenuAsset();
	void OnConfirm();

public:

	TSharedRef<SWidget> Construct(FOnAssetSelected onAssetConfirmed);

};
