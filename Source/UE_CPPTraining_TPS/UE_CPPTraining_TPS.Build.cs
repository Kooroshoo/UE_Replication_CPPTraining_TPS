// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_CPPTraining_TPS : ModuleRules
{
	public UE_CPPTraining_TPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
