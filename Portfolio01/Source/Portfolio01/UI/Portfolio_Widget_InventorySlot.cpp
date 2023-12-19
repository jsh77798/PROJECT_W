// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portfolio_Widget_InventorySlot.h"

void UPortfolio_Widget_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	ItemIconBack = Cast<UImage>(GetWidgetFromName(TEXT("ItemBack")));
	ItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));
	ItemCountBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));

	// 인벤토리 아이템에 있는 UInvenItemData는 인벤토리라면 무조건 가지고 있어야 합니다.
	ItemData = NewObject<UInventoryItemData>();
}

void UPortfolio_Widget_InventorySlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UPortfolio_Widget_InventorySlot::SetItemData(UInventoryItemData* _ItemData)
{
	ItemData = _ItemData;
	SlotDataCheck();
}

void UPortfolio_Widget_InventorySlot::SlotDataCheck()
{
	if (nullptr == ItemData)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == ItemData)"), __FUNCTION__, __LINE__);
		// 치명적인 에러
		//ItemBackVisibility = ESlateVisibility::Visible;
		//ItemIconVisibility = ESlateVisibility::Hidden;
		//ItemCountVisibility = ESlateVisibility::Hidden;
		return;
	}

	if (nullptr == ItemData->Data)
	{
		ItemBackVisibility = ESlateVisibility::Visible;
		ItemIconVisibility = ESlateVisibility::Hidden;
		ItemCountVisibility = ESlateVisibility::Hidden;
		return;
	}

	if (nullptr != ItemData->Data)
	{
		ItemIconVisibility = ESlateVisibility::Visible;
		ItemIconImage->SetBrushFromTexture(Cast<UTexture2D>(ItemData->Data->Icon));
		ItemCountValue = ItemData->Count;

		if (1 < ItemData->Data->StackMax)
		{
			ItemCountVisibility = ESlateVisibility::Visible;
		}
		else
		{
			ItemCountVisibility = ESlateVisibility::Hidden;
		}
	}

}

void UPortfolio_Widget_InventorySlot::DragSetting(UPortfolio_Widget_InventorySlot* _OtherDragSlot)
{
	ItemIconBack = Cast<UImage>(GetWidgetFromName(TEXT("ItemBack")));
	ItemIconImage = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));
	ItemCountBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemCount")));


	// 이걸호출되는 당사자는
	// 드래그를 위해서 만들어진 아이콘입니다.
	ItemBackVisibility = ESlateVisibility::Hidden;

	// _OtherDragSlot은 드래그의 시작이된 슬롯이다.
	SetItemData(_OtherDragSlot->ItemData);

}

void UPortfolio_Widget_InventorySlot::MoveSetting(UPortfolio_Widget_InventorySlot* _OtherDragSlot)
{
	if (_OtherDragSlot == this)
	{
		return;
	}

	// UInvenItemSlot* _OtherDragSlot 시작슬롯

	// 이걸 호출하는 쪽은 이제 도착 슬롯

	if (ItemTypeValue != ItemType::NONE && _OtherDragSlot->ItemData->Data->Type != ItemTypeValue)
	{
		return;
	}

	//const FItemData* SwapItemData = _OtherDragSlot->ItemData->Data;
	//_OtherDragSlot->ItemData->Data = ItemData->Data;
	//ItemData->Data = SwapItemData;

	UInventoryItemData* SwapItemData = _OtherDragSlot->ItemData;
	_OtherDragSlot->ItemData = ItemData;
	ItemData = SwapItemData;

	// 첫번째 함수포인터.
	// 두번째 다형성
	// StatusChracter

	// 여기 이부분에서 뭔가를 하고 싶어.
	// 그건 내가 알려줘야 하는데.
	// 함수포인터.
	// 나는 그냥 UI다. 나는 플레이어나 몬스터나 이런것들을 알고 싶지 않다.

	// 함수가 매핑 되었다면
	if (true == ItemChangeFunction.IsBound())
	{
		// 그 함수를 호출해라.
		ItemChangeFunction.Broadcast();
	}

	// 나는 다 받아서 세팅하고
	SlotDataCheck();
	_OtherDragSlot->SlotDataCheck();
}


