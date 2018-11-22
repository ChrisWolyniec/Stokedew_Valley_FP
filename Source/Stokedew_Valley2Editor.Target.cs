// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Stokedew_Valley2EditorTarget : TargetRules
{
	public Stokedew_Valley2EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("Stokedew_Valley2");
	}
}
