#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

#include "BehaviorTree/BlackboardData.h"
#include "Widgets/Input/SComboButton.h"

DECLARE_DELEGATE_OneParam(
	FOnSelectedBlackboardVarType,
	UClass*
)

class SSelectorBlackboardVarType
	: public SCompoundWidget
{
	static const FText Label;

	TSharedPtr<SComboButton> mpSelector;
	TSharedPtr<STextBlock> mpLabel;

	FOnSelectedBlackboardVarType OnSelected;

	TSharedRef<SWidget> BuildSelectMenu();
	void HandleKeyClassPicked(UClass* InClass);

public:

	SLATE_BEGIN_ARGS(SSelectorBlackboardVarType)
	{}
		SLATE_EVENT(FOnSelectedBlackboardVarType, OnSelected)
	SLATE_END_ARGS()

	void SetOnSelected(FOnSelectedBlackboardVarType InDelegate);

	void Construct(const FArguments& InArgs);

};

