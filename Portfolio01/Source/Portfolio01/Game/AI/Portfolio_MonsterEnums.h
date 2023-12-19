#pragma once

#include "CoreMinimal.h"

// ENUM정보-> Monster행동(애니메이션)
UENUM(BlueprintType)
enum class AIState : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	IDLE UMETA(DisplayName = "IDLE"),
	IDLE2 UMETA(DisplayName = "IDLE2"),
	IDLE3 UMETA(DisplayName = "IDLE3"),
	ROTATION UMETA(DisplayName = "ROTATION"),
	ROTATION2 UMETA(DisplayName = "ROTATION2"),
	MOVE UMETA(DisplayName = "MOVE"),
	RETURN UMETA(DisplayName = "RETURN"),
	PATROL UMETA(DisplayName = "PATROL"),
	ATTACK UMETA(DisplayName = "ATTACK"),
	ATTACK2 UMETA(DisplayName = "ATTACK2"),
	ATTACK3 UMETA(DisplayName = "ATTACK3"),
	MOVE_ATTACK UMETA(DisplayName = "MOVE_ATTACK"),
	MOVE_ATTACK2 UMETA(DisplayName = "MOVE_ATTACK2"),
	DEATH UMETA(DisplayName = "DEATH"),
	STUN UMETA(DisplayName = "STUN"),
	LEFT_LEG_STUN UMETA(DisplayName = "LEFT_LEG_STUN"),
	RIGHT_LEG_STUN UMETA(DisplayName = "RHIGT_LEG_STUN"),
	ABDOMEN_STUN UMETA(DisplayName = "ABDOMEN_STUN"),
	LEFT_ARM_STUN UMETA(DisplayName = "LEFT_ARM_STUN"),
	RIGHT_ARM_STUN UMETA(DisplayName = "RHIGT_ARM_STUN"),
};
