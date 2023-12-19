// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Portfolio_Global.h"
#include "Portfolio_GlobalObject.h"
#include "Portfolio_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UPortfolio_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPortfolio_GameInstance();
	~UPortfolio_GameInstance();

	// Get...함수-> 해당 데이터 가져오기
	UStaticMesh* GetMesh(FName _Name);

	const struct FItemData* GetRandomItemData();

	TSubclassOf<UObject> GetSubClass(FName _Name);

	struct FMonsterData* GetMonsterData(FName _Name);

	struct FPlayerData* GetPlayerData(FName _Name);

	struct FTileData* GetTileData(FName _Name);

	// GetGameData함수-> 무기 공격력 가져오기, SetGameData함수-> 무기 공격력 설정
	void GetGameData(int _Data, AActor* Owner);
	int SetGameData();
	int GameAtt = 0;
	AActor* _Owner = nullptr;

private:
	// SubClassData 테이블
	UPROPERTY()
	UDataTable* SubClassData;

	// MeshData 테이블
	UPROPERTY()
	UDataTable* MeshDatas;

	// ItemData 테이블
	UPROPERTY()
	UDataTable* ItemDatas;

	TArray<const struct FItemData*> ItemDataRandoms;

	// MonsterData 테이블
	UPROPERTY()
	UDataTable* MonsterDatas;

	// PlayerData 테이블
	UPROPERTY()
	UDataTable* PlayerDatas;

	// TileData 테이블
	UPROPERTY()
	UDataTable* TileDatas;

	TArray<UStaticMesh*> Arrmesh;

};
