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
		TArray<ECollisionTags> Collision_Tag;

	UPROPERTY(EditAnywhere)
		bool bCanMove = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float Hit_Stop = 0;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShakeBase> ShakeClass;
};
