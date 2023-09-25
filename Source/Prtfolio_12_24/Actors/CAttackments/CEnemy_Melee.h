#pragma once

#include "CoreMinimal.h"
#include "Actors/CAttackments/CAttackment.h"
#include "DataTable/FWeaponData_Enemy.h"

#include "CEnemy_Melee.generated.h"


UCLASS()
class PRTFOLIO_12_24_API ACEnemy_Melee : public ACAttackment
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Monster_Attack() override;
	virtual class UAnimMontage* Get_NextMontage() override;
	//UFUNCTION() // SetTimer  하려고
	//	virtual void Do_LAttack() override;
	//UFUNCTION()
	//	virtual void Do_RAttack() override;

	virtual void Begin_Attack() override;
	virtual void End_Attack() override;

	virtual void Button_Event(EMouseBut input) override;

	void Montage_Play(FWeaponData_Enemy input);
	void Montage_Play(FWeaponData_Enemy input, FName Section);

	virtual void Player_AttackSound() override;
	virtual void TakeOverlap_Actor(const FHitResult& Hit, FVector Hit_location, AActor* Causer, EAttackTag_Type AttackTag, float KnockBack_Power)  override;

	void Seach_ComboIndex(EMouseBut but);

	FWeaponData_Enemy LastDatas; // 공격에 맞았을 경우 Hit Stop 을 적용하려고 index 저장 개념
	FTransform Effect_Trans;	// 이펙트 위치 안맞을 경우를 위해
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FWeaponData_Enemy ComboData;	// 실질적 Combo Data 이걸로 Montage 사용
};
