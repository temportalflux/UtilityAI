#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "UtilityActionDetails.h"

class SUtilityAction
	: public SCompoundWidget
{
	static const FText TextCreateActionLabel;
	static const FText TextLabelBlackboardKey;

	FUtilityActionDetails mDetails;

	TSharedPtr<UBlackboardData> mpBlackboard;

	TSharedPtr<SVerticalBox> mpActionInputsBox;

	void AddInputField();

public:

	SLATE_BEGIN_ARGS(SUtilityAction)
		: _Index(0)
		, _BlackboardAsset(nullptr)
	{}

		SLATE_ATTRIBUTE(int32, Index)
		SLATE_ATTRIBUTE(TSharedPtr<UBlackboardData>, BlackboardAsset)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};
