#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Utilities/CStructs.h"
#include "Utilities/CEnums.h"

#include "FWeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAttack_Combo Data;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		EMouseBut Input_But;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float Power = 1.f;
	
};

USTRUCT(BlueprintType)
struct FFPSWeaponData : public FWeaponData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* FPS_Montage;

	UPROPERTY(EditAnywhere)
		float FPS_PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName FPS_StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		bool LastAttack = false;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShakeBase> ShakeClass;
};
