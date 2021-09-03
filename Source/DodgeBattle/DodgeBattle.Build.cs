// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DodgeBattle : ModuleRules
{
	public DodgeBattle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
