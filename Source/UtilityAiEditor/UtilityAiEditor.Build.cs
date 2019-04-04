// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UtilityAiEditor : ModuleRules
{
    public UtilityAiEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
				"UtilityAiEditor/Private",
                "Editor/GraphEditor/Private",
                "Editor/AIGraph/Private",
                "Editor/BehaviorTreeEditor/Private",
            }
            );

        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "AssetRegistry",
                "AssetTools",
                "PropertyEditor",
                "ContentBrowser",
                "BehaviorTreeEditor",
            }
        );
        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
				// ... add other public dependencies that you statically link with here ...
				"CoreUObject",
                "Engine",

                "AIModule",
                "GameplayTasks",

                "Slate",
                "SlateCore",

                "UtilityAi",
                "BehaviorTreeEditor",
                "UnrealEd",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
				// ... add private dependencies that you statically link with here ...
                "Projects",
                "Core",
                "CoreUObject",
                "LevelEditor",
                "ApplicationCore",
                "Engine",
                "RenderCore",
                "InputCore",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "UnrealEd",
                "AudioEditor",
                "MessageLog",
                "GraphEditor",
                "Kismet",
                "KismetWidgets",
                "PropertyEditor",
                "AnimGraph",
                "BlueprintGraph",
                "AIGraph",
                "AIModule",
                "ClassViewer",
                "BehaviorTreeEditor",
                "MainFrame",
            }
        );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ... 
                "WorkspaceMenuStructure",
                "AssetTools",
                "AssetRegistry",
                "ContentBrowser",
            }
            );
    }
}
