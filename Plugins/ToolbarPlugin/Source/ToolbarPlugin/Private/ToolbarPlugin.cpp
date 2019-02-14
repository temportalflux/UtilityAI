// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ToolbarPlugin.h"
#include "ToolbarPluginStyle.h"
#include "ToolbarPluginCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"

static const FName ToolbarPluginTabName("ToolbarPlugin");

#define LOCTEXT_NAMESPACE "FToolbarPluginModule"

void FToolbarPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FToolbarPluginStyle::Initialize();
	FToolbarPluginStyle::ReloadTextures();

	FToolbarPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FToolbarPluginCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FToolbarPluginModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FToolbarPluginModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FToolbarPluginModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FToolbarPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FToolbarPluginStyle::Shutdown();

	FToolbarPluginCommands::Unregister();
}

void FToolbarPluginModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FToolbarPluginModule::PluginButtonClicked()")),
							FText::FromString(TEXT("ToolbarPlugin.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FToolbarPluginModule::AddMenuExtension(FMenuBuilder& Builder)
{
	UE_LOG(LogSlate, Log, TEXT("Toolbar plugin build menu extension"));
	Builder.AddMenuEntry(FToolbarPluginCommands::Get().PluginAction);
}

void FToolbarPluginModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	UE_LOG(LogSlate, Log, TEXT("Toolbar plugin build toolbar extension"));
	Builder.AddToolBarButton(FToolbarPluginCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FToolbarPluginModule, ToolbarPlugin)