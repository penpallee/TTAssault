// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class TTAssaultServerTarget : TargetRules
{
	public TTAssaultServerTarget( TargetInfo Target) : base(Target)
	{
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange(new string[] { "TTAssault" });

		/*Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "TTAssault" } );*/
	}
}
