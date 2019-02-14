// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ToolbarCommands.h"

#define LOCTEXT_NAMESPACE "FUtilityAiEditorModule"

void FToolbarCommands::RegisterCommands()
{
	UI_COMMAND(Action_UtilityTree_OpenWizard, "UtilityAi", "Execute UtilityAi OpenWizard", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
