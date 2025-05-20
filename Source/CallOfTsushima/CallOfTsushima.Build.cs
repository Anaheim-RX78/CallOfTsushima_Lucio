// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CallOfTsushima : ModuleRules
{
	public CallOfTsushima(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Niagara", "Networking", "HTTP" });
	}
}
