#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Utilities/CStructs.h"
#include "Utilities/CEnums.h"

#include "FHitData.generated.h"


USTRUCT(BlueprintType)
struct FWeekHit_Data : public FTableRowBase // 약점 Montage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		EDirection direction;
	UPROPERTY(EditAnywhere)
		TArray<FName> WeekBone;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		class USoundCue* Hit_Sound;

	UPROPERTY(EditAnywhere)
		bool bCanMove = false;

};

USTRUCT(BlueprintType)
struct FHit_Data : public FTableRowBase	 // 공격 Type에 따른 Montage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		EDirection direction;
	UPROPERTY(EditAnywhere)
		EAttackTag_Type Tag;
	UPROPERTY(EditAnywhere)
		EStateType state = EStateType::Idle;  // 맞을때 현재 상태
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		class USoundCue* Hit_Sound;

	UPROPERTY(EditAnywhere)
		bool bCanMove = false;
};

USTRUCT(BlueprintType)
struct FParrying_Data : public FTableRowBase	 // 공격 Type에 따른 Montage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		EDirection direction;

	UPROPERTY(EditAnywhere)
		EStateType state = EStateType::Idle;  // 맞을때 현재 상태
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		class USoundCue* Hit_Sound;

	UPROPERTY(EditAnywhere)
		bool bCanMove = false;
};
