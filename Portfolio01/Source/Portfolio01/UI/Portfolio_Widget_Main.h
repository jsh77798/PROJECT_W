// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Portfolio_Widget_Main.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UPortfolio_Widget_Main : public UUserWidget
{
	GENERATED_BODY()

public:
	// ACharacter에 대한 참조
	//UPROPERTY(BlueprintReadOnly)
	//APortfolio_Character* MyCharacter;

	UFUNCTION(BlueprintCallable)
	int SetMyCharacter();
	
	UFUNCTION(BlueprintCallable)
	int SetMyCharacter2();

	//YourUserWidget->SetMyCharacter(GetOwningPlayerPawn());

	// 체력 값을 얻어옴
	//float CharacterHealth = YourUserWidget->MyCharacter->GetCharacterHealth();

	/*
	UFUNCTION(BlueprintCallable)
	int LoadTableRowData()
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("/Script/Engine.DataTable'/Game/BluePrint/Character_Player/DT_PlayerData.DT_PlayerData'"));
		if (DataTableAsset.Succeeded())
		{
			UDataTable* MyDataTable = DataTableAsset.Object;

			// 이제 MyDataTable을 사용하여 데이터를 읽을 수 있습니다.
			FPlayerData* MyDataRow = MyDataTable->FindRow<FPlayerData>(FName(TEXT("DT_PlayerData")), TEXT(""));
			if (MyDataRow)
			{
				int32 BulletCount = MyDataRow->Bullet;
		        return BulletCount;
			}
		}
	}
	*/
/*
	const int bullet = 1;
	FLinker* Src;

	UFUNCTION(BlueprintCallable)
	int SetCulBulletData()
	{
	    struct FPlayerData* CurPlayerData = Cast< FPlayerData>(Src);
		int Bullt = CurPlayerData->Bullet;
		return Bullt;
	}
*/
	UFUNCTION(BlueprintCallable)
		bool CheckAllWidGetHidden();

	UFUNCTION(BlueprintCallable)
		bool CheckAiming();
		
	UFUNCTION(BlueprintCallable)
		bool CheckRangeItem();

	UFUNCTION(BlueprintCallable)
		void SetOptionOnOffSwitch()
	{
		OptionOnOff = OptionOnOff == ESlateVisibility::Hidden ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	}

	void SetMapWindowOnOffSwitch()
	{
		MapWindowOnOff = MapWindowOnOff == ESlateVisibility::Hidden ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	}

	void SetInventoryOnOffSwitch()
	{
		InventoryOnOff = InventoryOnOff == ESlateVisibility::Hidden ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	}

	void SetW_BackGroundOnOffSwitch()
	{
		W_BackGroundOnOff = W_BackGroundOnOff == ESlateVisibility::Hidden ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	}

	void SetBButtonOnOffSwitch()
	{
		ButtonOnOff = ButtonOnOff == ESlateVisibility::Hidden ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	}

	void SetAimingOnOffSwitch()
	{
		AimingOnOff = AimingOnOff == ESlateVisibility::Hidden ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	}

	void SetAiming_ImageOnOffSwitch()
	{
		AimingOnOff = Aiming_ImageOnOff == ESlateVisibility::Hidden ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	}

protected:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


private:
	UPROPERTY(Category = "UIOnOff", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ESlateVisibility InventoryOnOff = ESlateVisibility::Hidden;

	UPROPERTY(Category = "UIOnOff", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ESlateVisibility MapWindowOnOff = ESlateVisibility::Hidden;

	UPROPERTY(Category = "UIOnOff", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ESlateVisibility OptionOnOff = ESlateVisibility::Hidden;

	UPROPERTY(Category = "UIOnOff", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ESlateVisibility W_BackGroundOnOff = ESlateVisibility::Hidden;

	UPROPERTY(Category = "UIOnOff", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ESlateVisibility ButtonOnOff = ESlateVisibility::Hidden;

	UPROPERTY(Category = "UIOnOff", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ESlateVisibility AimingOnOff = ESlateVisibility::Hidden;

	UPROPERTY(Category = "UIOnOff", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ESlateVisibility Aiming_ImageOnOff = ESlateVisibility::Hidden;

	class UInventoryUserWidget* Inven;

	UPROPERTY()
		TArray<UUserWidget*> AllWidGet;

	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool AllHidden = true;
};
