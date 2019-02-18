// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "WindowTest.h"
#include "WindowTestStyle.h"
#include "WindowTestCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

static const FName WindowTestTabName("WindowTest");

#define LOCTEXT_NAMESPACE "FWindowTestModule"

void FWindowTestModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FWindowTestStyle::Initialize();
	FWindowTestStyle::ReloadTextures();

	FWindowTestCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FWindowTestCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FWindowTestModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FWindowTestModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FWindowTestModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(WindowTestTabName, FOnSpawnTab::CreateRaw(this, &FWindowTestModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FWindowTestTabTitle", "WindowTest"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FWindowTestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FWindowTestStyle::Shutdown();

	FWindowTestCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(WindowTestTabName);
}

TSharedRef<SDockTab> FWindowTestModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FWindowTestModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("WindowTest.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FWindowTestModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(WindowTestTabName);
}

void FWindowTestModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FWindowTestCommands::Get().OpenPluginWindow);
}

void FWindowTestModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FWindowTestCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWindowTestModule, WindowTest)