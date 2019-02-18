// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UtilityAiEditor.h"

#include "ToolbarStyle.h"
#include "ToolbarCommands.h"
#include "Misc/MessageDialog.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "IMainFrameModule.h"
#include "SlateApplication.h"

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
	///*
	{
		this->mpBTToolbarExtender = (TSharedPtr<FExtender>)MakeShareable(new FExtender());
		this->mpBTToolbarExtender->AddToolBarExtension(
			"Asset",
			EExtensionHook::After,
			ToolbarCommands,
			FToolBarExtensionDelegate::CreateRaw(this, &FUtilityAiEditorModule::AddBTEditorToolbarExtension)
		);
		BTEditorModule.GetToolBarExtensibilityManager()->AddExtender(this->mpBTToolbarExtender);
	}
	//*/
	{
		this->mpBTMenuExtender = (TSharedPtr<FExtender>)MakeShareable(new FExtender());
		this->mpBTMenuExtender->AddMenuExtension(
			"WindowLayout",
			EExtensionHook::After,
			ToolbarCommands,
			FMenuExtensionDelegate::CreateRaw(this, &FUtilityAiEditorModule::AddMenuExtension)
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
	BTEditorModule.GetToolBarExtensibilityManager()->RemoveExtender(this->mpBTToolbarExtender);

	FToolbarStyle::Shutdown();
	FToolbarCommands::Unregister();
	
}

void FUtilityAiEditorModule::AddBTEditorToolbarExtension(FToolBarBuilder& Builder)
{
	UE_LOG(LogUtilityAiEditor, Log, TEXT("BT Utiltiy toolbar button time"));
	Builder.BeginSection("UtilityAi");
	{
		Builder.AddToolBarButton(FToolbarCommands::Get().Action_UtilityTree_OpenWizard);
	}
	Builder.EndSection();
}

void FUtilityAiEditorModule::AddMenuExtension(FMenuBuilder& Builder)
{
	UE_LOG(LogUtilityAiEditor, Log, TEXT("BT Utiltiy MENU button time"));
	Builder.AddMenuEntry(FToolbarCommands::Get().Action_UtilityTree_OpenWizard);
}

void FUtilityAiEditorModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	/*
	FText DialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
		FText::FromString(TEXT("FUtilityAiEditorModule::PluginButtonClicked()")),
		FText::FromString(TEXT("UtilityAiEditor.cpp"))
	);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
	//*/
	TSharedRef<SWindow> CookbookWindow = SNew(SWindow)
		.Title(FText::FromString(TEXT("Cookbook Window")))
		.ClientSize(FVector2D(800, 400))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Hello from Slate")))
		]
	];
	IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));

	if (MainFrameModule.GetParentWindow().IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(CookbookWindow, MainFrameModule.GetParentWindow().ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(CookbookWindow);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUtilityAiEditorModule, UtilityAiEditor)