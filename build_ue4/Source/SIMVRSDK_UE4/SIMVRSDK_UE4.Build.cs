// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SIMVRSDK_UE4 : ModuleRules
{
	public SIMVRSDK_UE4(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "SIMVRPlugin" });
	}
}
