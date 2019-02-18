// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "WindowTestStyle.h"

class FWindowTestCommands : public TCommands<FWindowTestCommands>
{
public:

	FWindowTestCommands()
		: TCommands<FWindowTestCommands>(TEXT("WindowTest"), NSLOCTEXT("Contexts", "WindowTest", "WindowTest Plugin"), NAME_None, FWindowTestStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};