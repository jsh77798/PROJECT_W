#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "TileData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct PORTFOLIO01_API FTileData : public FTableRowBase
{
	GENERATED_BODY()

		FTileData() {}
	~FTileData() {}

	// 언리얼 에디터에서 DT_PlayerData테이블에 들어가 해당설정값을 변경하고 설정할 수 있다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int RANGE_ATT;
};
