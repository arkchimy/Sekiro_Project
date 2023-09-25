#pragma once

#include "CoreMinimal.h"
#include "Utilities/CEnums.h"
#include "Engine/DataTable.h"
#include "GenericTeamAgentInterface.h" // Detection by Affiliation

#include "CStructs.generated.h"

/*
	장착 Equipment 
*/
USTRUCT(BlueprintType)
struct FEquipmentMontage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* FP_Montage;

	UPROPERTY(EditAnywhere)
		float FP_PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName FP_StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;
	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;

};
USTRUCT(BlueprintType)
struct FAttackMontage : public FEquipmentMontage
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<ECollisionTags> Tags;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Power = 5.0f;

	UPROPERTY(EditAnywhere)
		float HitStop;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShakeBase> ShakeClass;

};


/*
	 궁극기 FSpecialMontage
*/

USTRUCT(BlueprintType)
struct FSpecialMontage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Tag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* Try_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Try_PlayRatio = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* Resiste_Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Resiste_PlayRatio = 1.0f;
	UPROPERTY(EditAnywhere)
		bool bCanMove = false;
};

/*
	Attack Combo Data 구조체
*/
USTRUCT(BlueprintType)
struct FHittedEffect
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class USoundCue* HitSound;
};
USTRUCT(BlueprintType)
struct FAttack_Combo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		FHittedEffect HittedEffect;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;
	
};

USTRUCT(BlueprintType)
struct FPlayer_Combo : public FAttack_Combo
{
	GENERATED_BODY()

public:


	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShakeBase> ShakeClass;
};

USTRUCT(BlueprintType)
struct FFPS_Combo : public FAttack_Combo
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
		TSubclassOf<class UCameraShakeBase> ShakeClass;
};

USTRUCT(BlueprintType)
struct FEnemy_Combo : public FAttack_Combo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<ECollisionTags> Tags;

};

USTRUCT(BlueprintType)
struct FPlayer_Action : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAttack_Combo Jump_Key;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAttack_Combo Shift_F;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAttack_Combo Shift_B;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAttack_Combo Shift_L;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAttack_Combo Shift_R;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAttack_Combo SpaceBar_F;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAttack_Combo SpaceBar_B;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAttack_Combo SpaceBar_L;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAttack_Combo SpaceBar_R;

};

USTRUCT(BlueprintType)
struct FHit_Montage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

};
/*		
===============================   UI Cross Hair		===============================	
*/

USTRUCT(BlueprintType)
struct FCrossHairSet //  무기에 따른 CrossHair구조체
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* Cross_Hair;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UTexture2D* Side_Hair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Spread_Speed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* Hit_Hair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* Critical_Hair;
};
/*
===============================   Inventory	===============================
*/
USTRUCT(BlueprintType)
struct FInven_Item
{
	// GameActor has Item Struct
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int id = 0;

};



class PRTFOLIO_12_24_API CStructs
{

};
