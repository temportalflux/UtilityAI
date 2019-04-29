#include "SActionListing.h"

#include "UtilityAiEditor.h"

#include "Widgets/Input/SButton.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SActionListing"

void SActionListing::Construct(const FArguments& InArgs)
{
	this->mpAction = InArgs._Action.Get();
	this->mOnEdit = InArgs._OnEdit;
	this->mpAction.Pin()->mOnChanged.BindRaw(this, &SActionListing::OnActionChanged);

	ChildSlot
	[
		SAssignNew(mpContent, SHorizontalBox)

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

	mpNameLabel->SetText(FString("Action"));
}

void SActionListing::OnClickedEdit()
{
	this->mOnEdit.ExecuteIfBound(this->mpAction.Pin()->mIndex);
}

void SActionListing::OnActionChanged(TWeakPtr<FUtilityActionDetails> action)
{
	/*UE_LOG(LogUtilityAiEditor, Log, TEXT("Found change, updating action listing for %s, visible=%i"),
		*action.Pin()->mName.ToString(),
		action.Pin()->mShouldGenerate
	);*/
	this->mpContent->SetVisibility(this->mpAction.Pin()->mShouldGenerate ? EVisibility::Visible : EVisibility::Hidden);
	mpNameLabel->SetText(action.Pin()->mName.ToString());
}

#undef LOCTEXT_NAMESPACE