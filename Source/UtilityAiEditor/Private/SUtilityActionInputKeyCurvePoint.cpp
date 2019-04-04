#include "SUtilityActionInputKeyCurvePoint.h"

#include "Widgets/SBoxPanel.h"
#include "SBlackboardEntryInfo.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SUtilityActionInputKeyCurvePoint"

void SUtilityActionInputKeyCurvePoint::Construct(const FArguments & InArgs)
{
	this->mId = InArgs._Id.Get();
	this->mOnDelete = InArgs._OnDelete;
	this->mOnValueCommitted = InArgs._OnValueCommitted;

	ChildSlot
	[
		SNew(SVerticalBox)

			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("Remove", "Remove"))
				.OnPressed(FSimpleDelegate::CreateRaw(this, &SUtilityActionInputKeyCurvePoint::OnRemovePressed))
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
						SAssignNew(mpWidgetInput, SNumericEntryBox<float>)
						.Value(this, &SUtilityActionInputKeyCurvePoint::GetValueInput)
						.OnValueCommitted(SNumericEntryBox<float>::FOnValueCommitted::CreateRaw(
							this, &SUtilityActionInputKeyCurvePoint::OnCommittedValueInput))
					]

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
						.Text(LOCTEXT("OutputValue", "Utility Value"))
					]

					+SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoWidth()
					[
						SAssignNew(mpWidgetOutput, SNumericEntryBox<float>)
						.Value(this, &SUtilityActionInputKeyCurvePoint::GetValueOutput)
						.OnValueCommitted(SNumericEntryBox<float>::FOnValueCommitted::CreateRaw(
							this, &SUtilityActionInputKeyCurvePoint::OnCommittedValueOutput))
					]
			
			]

	];
}

void SUtilityActionInputKeyCurvePoint::OnRemovePressed()
{
	this->mOnDelete.ExecuteIfBound(this->mId);
}

TOptional<float> SUtilityActionInputKeyCurvePoint::GetValueInput() const
{
	return this->mData.input;
}

TOptional<float> SUtilityActionInputKeyCurvePoint::GetValueOutput() const
{
	return this->mData.output;
}

void SUtilityActionInputKeyCurvePoint::OnCommittedValueInput(float value, ETextCommit::Type type)
{
	this->mData.input = value;
	this->mOnValueCommitted.ExecuteIfBound(this->mId, this->mData);
}

void SUtilityActionInputKeyCurvePoint::OnCommittedValueOutput(float value, ETextCommit::Type type)
{
	this->mData.output = value;
	this->mOnValueCommitted.ExecuteIfBound(this->mId, this->mData);
}

#undef LOCTEXT_NAMESPACE 
