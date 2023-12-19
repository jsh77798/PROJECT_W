#pragma once

#include "CoreMinimal.h"

// ENUM정보-> Item종류
UENUM(BlueprintType)
enum class ItemType : uint8
{
	NONE UMETA(DisplayName = "NONE"),
	GUN UMETA(DisplayName = "GUN"),
	SHOTGUN UMETA(DisplayName = "SHOTGUN"),
	POSTION UMETA(DisplayName = "POSTION"),
};
