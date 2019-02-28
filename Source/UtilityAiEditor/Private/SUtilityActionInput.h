#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SButton.h"
#include "UtilityActionDetails.h"

DECLARE_DELEGATE_OneParam(FOnActionDelete, FGuid const &)

DECLARE_DELEGATE_TwoParams(FOnActionInputCommitted, FGuid const &, FUtilityActionInput const &)

class SUtilityActionInput
	: public SCompoundWidget
{
	static const FText TextLabelBlackboardKey;
	
	FGuid mId;
	TSharedPtr<UBlackboardData> mpBlackboard;

	FUtilityActionInput mData;

	FOnActionDelete mOnDelete;
	FOnActionInputCommitted mOnValueCommitted;

	void OnDelete();
	void OnChangedBlackboardKey(FUtilityActionEntry const &key);
	void OnCommittedInputValue(float value, ETextCommit::Type type);

public:

	SLATE_BEGIN_ARGS(SUtilityActionInput)
		: _Id(FGuid())
		, _BlackboardAsset(nullptr)
		, _OnDelete()
		, _OnValueCommitted()
	{}

		SLATE_ATTRIBUTE(FGuid, Id)
		
		SLATE_ATTRIBUTE(TSharedPtr<UBlackboardData>, BlackboardAsset)

		SLATE_EVENT(FOnActionDelete, OnDelete)

		SLATE_EVENT(FOnActionInputCommitted, OnValueCommitted)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};
