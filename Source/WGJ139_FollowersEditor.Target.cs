// © Master Kenth 2020

using UnrealBuildTool;
using System.Collections.Generic;

public class WGJ139_FollowersEditorTarget : TargetRules
{
	public WGJ139_FollowersEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "WGJ139_Followers" } );
	}
}
