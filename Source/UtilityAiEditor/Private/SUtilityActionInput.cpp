#include "SUtilityActionInput.h"

#include "Widgets/SBoxPanel.h"
#include "SBlackboardEntryInfo.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SUtilityActionInput"

const FText SUtilityActionInput::TextLabelBlackboardKey = LOCTEXT("BlackboardKey", "Blackboard Key");

void SUtilityActionInput::Construct(const FArguments & InArgs)
{
	this->mId = InArgs._Id.Get();
	this->mpBlackboard = InArgs._BlackboardAsset.Get();
	this->mOnDelete = InArgs._OnDelete;
	this->mOnValueCommitted = InArgs._OnValueCommitted;

	// TODO: Utility Value (probablity)
	//		low (25%), medium (50%), high (75%), definite (500%)
	// TODO: Curve Type
	//		constant, smooth, ???
	ChildSlot
	[
		SNew(SVerticalBox)

			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
			
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(TextLabelBlackboardKey)
					]
			
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoWidth()
					[
						SNew(SButton)
						.Text(LOCTEXT("DeleteEntry", "Remove"))
						.OnPressed(FSimpleDelegate::CreateRaw(this, &SUtilityActionInput::OnDelete))
					]
			]

			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SVerticalBox)

					+SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoHeight()
					[
						SNew(SBlackboardEntryInfo)
						.BlackboardAsset(this->mpBlackboard)
						.OnChanged(FOnBlackboardEntryChanged::CreateRaw(this, &SUtilityActionInput::OnChangedBlackboardKey))
					]

					+SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoHeight()
					[
						SNew(SHorizontalBox)

							+SHorizontalBox::Slot()
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Top)
							.AutoWidth()
							[
								SNew(STextBlock)
								.Text(LOCTEXT("InputValue", "Input Value"))
							]

							+SHorizontalBox::Slot()
							.HAlign(HAlign_Center)
							.VAlign(VAlign_Top)
							.AutoWidth()
							[
								SAssignNew(mpWidgetInputValue, SNumericEntryBox<float>)
								.Value(this, &SUtilityActionInput::GetInputValue)
								.OnValueCommitted(SNumericEntryBox<float>::FOnValueCommitted::CreateRaw(
									this, &SUtilityActionInput::OnCommittedInputValue))
							]

					]
			]

	];
}

TOptional<float> SUtilityActionInput::GetInputValue() const
{
	return this->mData.mInputValue;
}

void SUtilityActionInput::OnDelete()
{
	this->mOnDelete.ExecuteIfBound(this->mId);
}

void SUtilityActionInput::OnChangedBlackboardKey(FUtilityActionEntry const &key)
{
	mData.mBlackboardKeyEntry = key;
	this->mOnValueCommitted.ExecuteIfBound(this->mId, this->mData);
}

void SUtilityActionInput::OnCommittedInputValue(float value, ETextCommit::Type type)
{
	mData.mInputValue = value;
	this->mOnValueCommitted.ExecuteIfBound(this->mId, this->mData);
}

#undef LOCTEXT_NAMESPACE 
