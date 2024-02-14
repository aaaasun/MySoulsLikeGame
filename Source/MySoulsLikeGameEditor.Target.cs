// copyright asun

using UnrealBuildTool;
using System.Collections.Generic;

public class MySoulsLikeGameEditorTarget : TargetRules
{
	public MySoulsLikeGameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "MySoulsLikeGame" } );
	}
}
