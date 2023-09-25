#pragma once
#include "CoreMinimal.h"
#include "FWeaponData.h"

#include "FWeaponData_Player.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData_Player : public FWeaponData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		bool bCanMove = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float Hit_Stop = 0;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShakeBase> ShakeClass;
};
