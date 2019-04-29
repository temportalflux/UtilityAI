#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "UtilityActionDetails.h"
#include "SUtilityActionInputKeyCurvePoint.h"

DECLARE_DELEGATE_TwoParams(FOnActionInputCommitted, FGuid const &, FUtilityActionInput const &)

/**
* An input for a UtilityAction (configurable key and data for curve)
*/
class SUtilityActionInput
	: public SCompoundWidget
{
	static const FText TextLabelBlackboardKey;
	
	UBlackboardData* mpBlackboard;

	FGuid mId;
	FUtilityActionInput mData;

	TSharedPtr<SVerticalBox> mpWidgetBoxCurveKeys;
	TMap<FGuid, TSharedPtr<SUtilityActionInputKeyCurvePoint>> mpWidgetCurveKeys;

	FOnActionDelete mOnDelete;
	FOnActionInputCommitted mOnValueCommitted;

	void OnDelete();
	void AddCurveKey();
	void OnChangedBlackboardKey(FUtilityActionEntry const &key);
	void OnValueCommittedCurveKey(FGuid const &id, FUtilityActionInputCurveKey const &value);
	void RemoveCurveKey(FGuid const &id);

public:

	SLATE_BEGIN_ARGS(SUtilityActionInput)
		: _Id(FGuid())
		, _BlackboardAsset(nullptr)
		, _OnDelete()
		, _OnValueCommitted()
	{}

		SLATE_ATTRIBUTE(FGuid, Id)
		
		SLATE_ATTRIBUTE(UBlackboardData*, BlackboardAsset)

		SLATE_EVENT(FOnActionDelete, OnDelete)

		SLATE_EVENT(FOnActionInputCommitted, OnValueCommitted)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};
