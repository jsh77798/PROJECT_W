#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include <Global/Portfolio_GlobalEnums.h>
#include "Engine/Texture.h"
#include "Engine/StaticMesh.h"
#include "ItemData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct PORTFOLIO01_API FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FItemData() {}
	~FItemData() {}

	// 언리얼 에디터에서 DT_ItemData테이블에 들어가 해당설정값을 변경하고 설정할 수 있다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		ItemType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int StackMax = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int Att;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int VALUE;
};