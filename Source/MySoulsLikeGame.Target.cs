// copyright asun

using UnrealBuildTool;
using System.Collections.Generic;

public class MySoulsLikeGameTarget : TargetRules
{
	public MySoulsLikeGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "MySoulsLikeGame" } );
	}
}
