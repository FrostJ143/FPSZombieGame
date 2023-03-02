// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSZombie : ModuleRules
{
	public FPSZombie(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "PhysicsCore" });
		PublicDependencyModuleNames.AddRange(new string[] {"Slate", "SlateCore"});
	}
}
