// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UtilityAiEditor.h"

#include "ToolbarStyle.h"
#include "ToolbarCommands.h"
#include "Misc/MessageDialog.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "IMainFrameModule.h"
#include "SlateApplication.h"
#include "DetailLayoutBuilder.h"

#include "BehaviorTreeEditorModule.h"

DEFINE_LOG_CATEGORY(LogUtilityAiEditor);

#define LOCTEXT_NAMESPACE "FUtilityAiEditorModule"

void FUtilityAiEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogUtilityAiEditor, Log, TEXT("Loading UtilityAiEditor..."));

	FToolbarStyle::Initialize();
	FToolbarStyle::ReloadTextures();

	FToolbarCommands::Register();

	ToolbarCommands = MakeShareable(new FUICommandList);

	ToolbarCommands->MapAction(
		FToolbarCommands::Get().Action_UtilityTree_OpenWizard,
		FExecuteAction::CreateRaw(this, &FUtilityAiEditorModule::PluginButtonClicked),
		FCanExecuteAction()
	);

	// https://answers.unrealengine.com/questions/25609/customizing-the-editors-toolbar-buttons-menu-via-c.html

	FBehaviorTreeEditorModule& BTEditorModule = FModuleManager::LoadModuleChecked<FBehaviorTreeEditorModule>("BehaviorTreeEditor");
	{
		this->mpBTMenuExtender = (TSharedPtr<FExtender>)MakeShareable(new FExtender());
		this->mpBTMenuExtender->AddMenuExtension(
			"WindowLayout",
			EExtensionHook::After,
			ToolbarCommands,
			FMenuExtensionDelegate::CreateLambda([](FMenuBuilder& Builder) {
				Builder.AddMenuEntry(FToolbarCommands::Get().Action_UtilityTree_OpenWizard);
			})
		);
		BTEditorModule.GetMenuExtensibilityManager()->AddExtender(this->mpBTMenuExtender);
	}

}

void FUtilityAiEditorModule::ShutdownModule()
{
	UE_LOG(LogUtilityAiEditor, Log, TEXT("Unloading UtilityAiEditor..."));
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FBehaviorTreeEditorModule& BTEditorModule = FModuleManager::LoadModuleChecked<FBehaviorTreeEditorModule>("BehaviorTreeEditor");
	BTEditorModule.GetMenuExtensibilityManager()->RemoveExtender(this->mpBTMenuExtender);

	FToolbarStyle::Shutdown();
	FToolbarCommands::Unregister();
	
}

void OpenWindow(TSharedRef<SWindow> window)
{
	IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));

	if (MainFrameModule.GetParentWindow().IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(window, MainFrameModule.GetParentWindow().ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(window);
	}
}

void FUtilityAiEditorModule::PluginButtonClicked()
{
	CurrentItem = MakeShareable(new FComboTest{ LOCTEXT("CreateNew", "New Blackboard Key") });
	Options.Add(CurrentItem);
	Options.Add(MakeShareable(new FComboTest{ LOCTEXT("A", "A") }));

	OpenWindow(
		SNew(SWindow)
			.Title(FText::FromString(TEXT("Create Utility Tree")))
			.ClientSize(FVector2D(800, 400))
			.SupportsMaximize(false)
			.SupportsMinimize(false)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Top)
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Create Action")))
					]
				+SVerticalBox::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Top)
					[
						SNew(SButton)
						.Text(FText::FromString(TEXT("This is a button")))
					]
				+ SVerticalBox::Slot()
					.HAlign(HAlign_Left)
					.VAlign(VAlign_Top)
					[
						SNew(SComboBox<TSharedPtr<FComboTest>>)
							.OptionsSource(&Options)
							.OnSelectionChanged_Lambda([&](TSharedPtr<FComboTest> Item, ESelectInfo::Type){
								CurrentItem = Item;
								UpdateProperty();
							})
							.OnGenerateWidget_Lambda([&](TSharedPtr<FComboTest> Item){
								return SNew(STextBlock)
									.Font(IDetailLayoutBuilder::GetDetailFont())
									.Text(Item->Label);
							})
							.InitiallySelectedItem(CurrentItem)
							[
								SAssignNew(CurrentText, STextBlock)
									.Font(IDetailLayoutBuilder::GetDetailFont())
									.Text(CurrentItem->Label)
							]
					]
			]
	);
}

void FUtilityAiEditorModule::UpdateProperty()
{
	if (CurrentItem == Options.Last())
	{
		// Show custom loop entry
		//LoopEntry->SetVisibility(EVisibility::Visible);
	}
	else
	{
		// Hide custom loop entry
		//LoopEntry->SetVisibility(EVisibility::Collapsed);
	}

	//LoopCountProperty->SetValue(CurrentItem->Value);
	CurrentText->SetText(CurrentItem->Label);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUtilityAiEditorModule, UtilityAiEditor)