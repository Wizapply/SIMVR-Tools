// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class SIMVRPlugin : ModuleRules
	{
		public SIMVRPlugin(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
                    "SIMVRPlugin/Public"
                }
				);

			PrivateIncludePaths.AddRange(
				new string[] {
                    "SIMVRPlugin/Private",
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
                    "Engine",
                    "Core",
                    "CoreUObject",
                    "Projects",
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					// ... add private dependencies that you statically link with here ...
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
		}
	}
}