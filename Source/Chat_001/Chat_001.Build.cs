// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Chat_001 : ModuleRules
{
	public Chat_001(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG"
			, "Slate", "SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		

	}
}
