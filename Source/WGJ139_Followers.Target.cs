// © Master Kenth 2020

using UnrealBuildTool;
using System.Collections.Generic;

public class WGJ139_FollowersTarget : TargetRules
{
	public WGJ139_FollowersTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "WGJ139_Followers" } );
	}
}
