#pragma once
#include "CoreMinimal.h"
#include "FWeaponData.h"

#include "FWeaponData_Enemy.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData_Enemy : public FWeaponData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		ECombat_Monster Combat;
};
