// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <Global/Data/ItemData.h>
#include "InventoryItemData.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UInventoryItemData : public UObject
{
	GENERATED_BODY()

public:
	class UPortfolio_Widget_InventorySlot* Widget;
	int Count = 1;
	const FItemData* Data;
};
