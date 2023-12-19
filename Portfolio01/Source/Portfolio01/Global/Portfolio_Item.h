// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Global/Data/ItemData.h>
#include "Portfolio_Item.generated.h"

UCLASS()
class PORTFOLIO01_API APortfolio_Item : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortfolio_Item();
	//날 누군가 먹으면
	UFUNCTION(BlueprintCallable)
	void Take(AActor* _Actor);

	//UFUNCTION(BlueprintCallable)
	//void SetItemCheckOnOffSwitch()
	//{
	//  ItemCheckOnOff = ItemCheckOnOff == ESlateVisibility::Hidden ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	//}

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:	
	const struct FItemData* Data = nullptr;

	//UPROPERTY(Category = "UIOnOff", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//ESlateVisibility ItemCheckOnOff = ESlateVisibility::Hidden;
};
