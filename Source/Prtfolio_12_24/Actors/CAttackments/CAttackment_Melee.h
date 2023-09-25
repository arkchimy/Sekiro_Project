#pragma once

#include "CoreMinimal.h"
#include "Actors/CAttackments/CAttackment.h"
#include "DataTable/FWeaponData_Player.h"

#include "CAttackment_Melee.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API ACAttackment_Melee : public ACAttackment
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	FORCEINLINE virtual void Enable_Combo() override { Nextable = true; }
	FORCEINLINE virtual void Unable_Combo() override { Nextable = false;}

	virtual void NextAttack_Play() override;

	//============= attack ����
	UFUNCTION() // SetTimer  �Ϸ���
		virtual void Do_LAttack() override;
	UFUNCTION()
		virtual void Do_RAttack() override; // Right But
	virtual void Do_Parrying() override; // Only GuardMode
	virtual void SuddenAttack()override;

	virtual void T_Attack() override;
	virtual void R_Attack() override; // Ű���� R

	virtual void L_AttackRelease() override;
	virtual void R_AttackRelease() override;
	virtual void Pressed_But() override;

	virtual void Begin_Attack() override;
	virtual void End_Attack() override;

	virtual void Button_Event(EMouseBut input) override;

	void Montage_Play(FWeaponData_Player input);
	void Montage_Play(FWeaponData_Player input,FName Section);
	//==================
	virtual void Player_AttackSound() override;
	virtual void TakeOverlap_Actor(const FHitResult& Hit, FVector Hit_location,AActor* Causer, EAttackTag_Type AttackTag, float KnockBack_Power) override;

	void Seach_ComboIndex(EMouseBut but);
private:// �Ѽհ� ��Ʈ Ư���� ����� Forward ����.
	//void Forward_Rush();
	UFUNCTION()
		void Dilation_Return();
	float Rush_Speed = 100.0f;
private:
	bool R_push_Chk = false;
	bool L_push_Chk = false;


	FWeaponData_Player LastDatas; // ���ݿ� �¾��� ��� Hit Stop �� �����Ϸ��� index ���� ����
	FTransform Effect_Trans;	// ����Ʈ ��ġ �ȸ��� ��츦 ����
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FWeaponData_Player ComboData;	// ������ Combo Data �̰ɷ� Montage ���

	
};
