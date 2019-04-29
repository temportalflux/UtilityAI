#include "SUtilityAction.h"
#include "UtilityAiEditor.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "SUtilityActionInput.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SUtilityAction"

void SUtilityAction::Construct(const FArguments& InArgs)
{
	this->mpDetails = InArgs._Value.Get();
	this->mpBlackboard = InArgs._BlackboardAsset.Get();
	this->mfOnDelete = InArgs._OnDelete;

	this->mpDetails.Pin()->mName = FName(*FText::FormatNamed(
		FText::FromString(TEXT("Action {index}")),
		TEXT("index"),
		this->mpDetails.Pin()->mIndex).ToString()
	);

	ChildSlot
	[
		SNew(SVerticalBox)

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

					// Text "Name:"
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.AutoWidth()
					.Padding(5.0f)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("UtilityTreeWizard_ActionName", "Action Name"))
					]

					// User input action name
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.FillWidth(1.0f)
					.Padding(5.0f)
					[
						SNew(SEditableTextBox)
						.Text(FText::FromName(this->mpDetails.Pin()->mName))
						.OnTextCommitted(FOnTextCommitted::CreateRaw(this, &SUtilityAction::OnNameCommitted))
					]

					// Text "Name:"
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.AutoWidth()
					.Padding(5.0f)
					[
						SNew(SButton)
						.Text(LOCTEXT("UtilityTreeWizard_ActionDelete", "Remove Action"))
						.ButtonColorAndOpacity(FSlateColor(FLinearColor(0.8, 0, 0)))
						.OnPressed(FSimpleDelegate::CreateRaw(this, &SUtilityAction::OnPressedDelete))
					]
			]

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

					+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.AutoWidth()
					.Padding(5.0f)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Inputs", "Inputs"))
					]

					+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.AutoWidth()
					.Padding(5.0f)
					[
						SNew(SButton)
						.Text(LOCTEXT("AddEntry", "+ Input"))
						.OnPressed(FSimpleDelegate::CreateRaw(this, &SUtilityAction::AddInputField))
					]

			]

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.AutoHeight()
			.Padding(20.0f, 0, 0, 0)
			[
				SAssignNew(mpActionInputsBox, SVerticalBox)
			]

	];
}

void SUtilityAction::OnNameCommitted(FText const &text, ETextCommit::Type commitType)
{
	if (!this->mpDetails.IsValid()) return;
	this->mpDetails.Pin()->mName = FName(*text.ToString());
	this->mpDetails.Pin()->mOnChanged.ExecuteIfBound(mpDetails);
}

void SUtilityAction::AddInputField()
{
	if (!this->mpDetails.IsValid()) return;

	FGuid id = FGuid::NewGuid();
	
	this->mpDetails.Pin()->mInputs.Add(id, FUtilityActionInput());

	TSharedPtr<SWidget> tmp;
	this->mpActionInputsBox->AddSlot()
		.AutoHeight()
	[
		SAssignNew(tmp, SUtilityActionInput)
		.Id(id)
		.BlackboardAsset(this->mpBlackboard)
		.OnDelete(FOnActionDelete::CreateRaw(this, &SUtilityAction::RemoveInputField))
		.OnValueCommitted(FOnActionInputCommitted::CreateRaw(this, &SUtilityAction::OnInputValueCommitted))
	];

	mInputWidgets.Add(id, tmp);

	this->mpDetails.Pin()->mOnChanged.ExecuteIfBound(mpDetails);
}

void SUtilityAction::RemoveInputField(FGuid const &id)
{
	if (!this->mpDetails.IsValid()) return;

	this->mpActionInputsBox->RemoveSlot(this->mInputWidgets[id].ToSharedRef());
	this->mInputWidgets.Remove(id);
	this->mpDetails.Pin()->mInputs.Remove(id);

	this->mpDetails.Pin()->mOnChanged.ExecuteIfBound(mpDetails);
}

void SUtilityAction::OnInputValueCommitted(FGuid const &id, FUtilityActionInput const &data)
{
	if (!this->mpDetails.IsValid()) return;
	this->mpDetails.Pin()->mInputs[id] = data;
	this->mpDetails.Pin()->mOnChanged.ExecuteIfBound(mpDetails);
}

void SUtilityAction::OnPressedDelete()
{
	if (!this->mpDetails.IsValid()) return;
	this->mpDetails.Pin()->mShouldGenerate = false;
	this->mpDetails.Pin()->mOnChanged.ExecuteIfBound(mpDetails);
	this->mfOnDelete.ExecuteIfBound();
}

#undef LOCTEXT_NAMESPACE