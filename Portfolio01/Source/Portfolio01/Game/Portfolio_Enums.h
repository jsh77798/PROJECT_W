#pragma once

#include "CoreMinimal.h"

// ENUM정보-> Player행동(애니메이션)
UENUM(BlueprintType)
enum class EAniState : uint8
{
	None UMETA(DisplayName = "None"),
	Idle UMETA(DisplayName = "Idle"),
	LeftMove UMETA(DisplayName = "LeftMove"),
	RightMove UMETA(DisplayName = "RightMove"),
	ForwardMove UMETA(DisplayName = "ForwardMove"),
	BackwardMove UMETA(DisplayName = "BackwardMove"),
	W_Aiming UMETA(DisplayName = "W_Aiming"),
	Aiming_LeftMove UMETA(DisplayName = "Aiming_LeftMove"),
	Aiming_RightMove UMETA(DisplayName = "Aiming_RightMove"),
	Aiming_ForwardMove UMETA(DisplayName = "Aiming_ForwardMove"),
	Aiming_BackwardMove UMETA(DisplayName = "Aiming_BackwardMove"),
	W_Attack UMETA(DisplayName = "W_Attack"),
	LeftRun UMETA(DisplayName = "LeftRun"),
	RightRun UMETA(DisplayName = "RightRun"),
	ForwardRun UMETA(DisplayName = "ForwardRun"),
	BackwardRun UMETA(DisplayName = "BackwardRun"),
	CrouchOn UMETA(DisplayName = "CrouchOn"),
	CrouchOff UMETA(DisplayName = "CrouchOff"),
	Crouch_Idle UMETA(DisplayName = "Crouch_Idle"),
	Crouch_LeftMove UMETA(DisplayName = "Crouch_LeftMove"),
	Crouch_RightMove UMETA(DisplayName = "Crouch_RightMove"),
	Crouch_ForwardMove UMETA(DisplayName = "Crouch_ForwardMove"),
	Crouch_BackwardMove UMETA(DisplayName = "Crouch_BackwardMove"),
	Hit UMETA(DisplayName = "Hit"),
	Load UMETA(DisplayName = "Load"),
};

/*
Run UMETA(DisplayName = "Run"),


W_Idle UMETA(DisplayName = "W_Idle"),
W_LeftMove UMETA(DisplayName = "W_LeftMove"),
W_RightMove UMETA(DisplayName = "W_RightMove"),
W_ForwardMove UMETA(DisplayName = "W_ForwardMove"),
W_BackwardMove UMETA(DisplayName = "W_BackwardMove"),
W_Run UMETA(DisplayName = "W_Run"),
W_Crouch UMETA(DisplayName = "W_Crouch"),
W_CrouchUP UMETA(DisplayName = "W_CrouchUP"),
*/

/*
UENUM(BlueprintType)
enum class AIState : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	IDLE UMETA(DisplayName = "IDLE"),
	MOVE UMETA(DisplayName = "MOVE"),
	PATROL UMETA(DisplayName = "PATROL"),
	RETURN UMETA(DisplayName = "RETURN"),
	ATTACK UMETA(DisplayName = "ATTACK"),
	DEATH UMETA(DisplayName = "DEATH"),
};
*/