#include "SActionListing.h"

#include "UtilityAiEditor.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SActionListing"

void SActionListing::Construct(const FArguments& InArgs)
{
	this->mpAction = InArgs._Action.Get();
	this->mOnEdit = InArgs._OnEdit;
	this->mpAction.Pin()->mOnChanged.BindRaw(this, &SActionListing::OnActionChanged);

	ChildSlot
	[
		SNew(SHorizontalBox)

		+SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		.FillWidth(1.0f)
		[
			SAssignNew(mpNameLabel, STextBlock)
		]
	
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.AutoWidth()
		.Padding(10.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("Edit", "Edit"))
			.OnPressed(FSimpleDelegate::CreateRaw(this, &SActionListing::OnClickedEdit))
		]


	];

	mpNameLabel->SetText(FString("Action ?"));
}

void SActionListing::OnClickedEdit()
{
	UE_LOG(LogUtilityAiEditor, Log, TEXT("Updating listing for editted action"));
	this->mOnEdit.ExecuteIfBound(this->mpAction.Pin()->mIndex);
}

void SActionListing::OnActionChanged(TWeakPtr<FUtilityActionDetails> action)
{
	mpNameLabel->SetText(action.Pin()->mName.ToString());
}

#undef LOCTEXT_NAMESPACE