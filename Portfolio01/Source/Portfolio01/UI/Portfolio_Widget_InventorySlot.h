// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Global/Data/ItemData.h>
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include <UI/InventoryItemData.h>
#include "Portfolio_Widget_InventorySlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotFunction);

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UPortfolio_Widget_InventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite)
		ESlateVisibility ItemBackVisibility = ESlateVisibility::Visible;

	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite)
		ESlateVisibility ItemIconVisibility = ESlateVisibility::Hidden;

	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite)
		ESlateVisibility ItemCountVisibility = ESlateVisibility::Hidden;

	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite)
		ItemType ItemTypeValue = ItemType::NONE;

	UPROPERTY(Category = "Icon", EditAnywhere, BlueprintReadWrite)
		UImage* ItemIconImage = nullptr;

	UPROPERTY(Category = "Icon", EditAnywhere, BlueprintReadWrite)
		UImage* ItemIconBack = nullptr;

	UPROPERTY(Category = "Count", EditAnywhere, BlueprintReadWrite)
		UTextBlock* ItemCountBlock = nullptr;

	UPROPERTY(Category = "Count", EditAnywhere, BlueprintReadWrite)
		int ItemCountValue = 0;

	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	FORCEINLINE UInventoryItemData* GetItemDataObject()
	{
		return ItemData;
	}

	void SetItemData(UInventoryItemData* _ItemData);

	void SlotDataCheck();

	UFUNCTION(BlueprintCallable)
		void DragSetting(UPortfolio_Widget_InventorySlot* _OtherDragSlot);

	UFUNCTION(BlueprintCallable)
		void MoveSetting(UPortfolio_Widget_InventorySlot* _OtherDragSlot);

	UFUNCTION(BlueprintCallable)
		bool IsEmpty()
	{
		return ItemData->Data == nullptr;
	}

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
		FSlotFunction ItemChangeFunction;

	UFUNCTION(BlueprintCallable)
		UInventoryItemData* GetItemData()
	{
		return ItemData;
	}

private:
	UInventoryItemData* ItemData = nullptr;


	// UStatusActor* Player;
	// UAIChracter* Player;
	// UMonster* Player;
	// UNpc* Player;
	// USummon* Player;

	// C++코딩으로만 처리가 가능.
	// std::function<>

	// 1. 함수 포인터를 가질것이냐 입니다.
	// 2. StatusChracter* Player;
};
