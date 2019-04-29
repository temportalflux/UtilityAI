#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "UtilityActionDetails.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "SActionListing.h"

DECLARE_DELEGATE_OneParam(FOnAddAction, TSharedRef<FUtilityActionDetails>)

class SWizardSidebar
	: public SCompoundWidget
{
private:
	TSharedPtr<SWidget> mpButtonFinish, mpButtonAddAction;
	TSharedPtr<SVerticalBox> mpActionListingsPanel;

	FOnAddAction mOnAddAction;
	FOnEditAction mOnEditAction;
	TArray<FUtilityActionDetails> *mpActions;

	void AddAction();

public:
	
	SLATE_BEGIN_ARGS(SWizardSidebar)
		: _Actions(nullptr)
		, _OnFinish()
		, _OnAddAction()
		, _OnEditAction()
	{}

		SLATE_ATTRIBUTE(TArray<FUtilityActionDetails>*, Actions)
		SLATE_EVENT(FSimpleDelegate, OnFinish)
		SLATE_EVENT(FOnAddAction, OnAddAction)
		SLATE_EVENT(FOnEditAction, OnEditAction)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};
