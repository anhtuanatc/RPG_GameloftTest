// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPG_GameloftTest : ModuleRules
{
	public RPG_GameloftTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem", "Niagara" });
	}
}
