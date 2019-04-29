#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "Widgets/Text/STextBlock.h"

#include "UtilityActionDetails.h"

DECLARE_DELEGATE_OneParam(FOnEditAction, int32 const)

class SActionListing
	: public SCompoundWidget
{
private:
	TSharedPtr<STextBlock> mpNameLabel;
	TSharedPtr<SWidget> mpButtonEdit;

	TWeakPtr<FUtilityActionDetails> mpAction;
	FOnEditAction mOnEdit;

	void OnClickedEdit();
	void OnActionChanged(TWeakPtr<FUtilityActionDetails> action);

public:

	SLATE_BEGIN_ARGS(SActionListing)
		: _Action(nullptr)
		, _OnEdit()
	{}

		SLATE_ATTRIBUTE(TWeakPtr<FUtilityActionDetails>, Action)
		SLATE_EVENT(FOnEditAction, OnEdit)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};
