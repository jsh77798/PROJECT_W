#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include <Game/AI/Portfolio_MonsterEnums.h>
#include "MonsterData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct PORTFOLIO01_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()

	FMonsterData(){}
	~FMonsterData(){}

	// 언리얼 에디터에서 DT_MonsterData테이블에 들어가 해당설정값을 변경하고 설정할 수 있다
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") //HP설정
	    int HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") //ATT설정
		int ATT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") //AI설정
		class UBehaviorTree* AI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") //애니메이션 설정
		TMap<AIState, class UAnimMontage*> MapAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float SearchRange = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AttackRange = 200.0f;
};
