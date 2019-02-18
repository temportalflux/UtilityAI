// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "WindowTestCommands.h"

#define LOCTEXT_NAMESPACE "FWindowTestModule"

void FWindowTestCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "WindowTest", "Bring up WindowTest window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
