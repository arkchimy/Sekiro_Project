#pragma once

#include "CoreMinimal.h"
#include "Actors/CAttackments/CAttackment.h"
#include "Utilities/CEnums.h"
#include "Utilities/CStructs.h"
#include "DataTable/FWeaponData.h"

#include "CAttackment_Archer.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API ACAttackment_Archer : public ACAttackment
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	virtual void Enable_Combo() override;
	virtual void Unable_Combo() override;
	virtual void NextAttack_Play() override;

	virtual void Do_LAttack() override;
	virtual void Do_RAttack() override;

	virtual void L_AttackRelease() override;
	virtual void R_AttackRelease() override;

	virtual void Begin_Attack() override;
	virtual void End_Attack() override;


	virtual void T_Attack() override;
	virtual void T_AttackRelease() override;

	UFUNCTION()
		virtual void OnBeginOverlap(const FHitResult& Hit,float power,AActor* Causer);
public:
	UFUNCTION()
		void ZoomAction();
	UFUNCTION()
		void PressedAction();
	UFUNCTION()
		void DrawDebug_Arc();
	virtual void Reload_Arraw() override;

	UFUNCTION()
		void Reload_Arraw_Loop();
private:
	float ZoomVal = 90;
	float PressVal = 0;
private: 
	//TSubclassOf<class ACThrowItem> Throw_Class;
private:
	class ACThrowItem* causer;
	FEquipmentMontage Reload_Montage;
	class ACThrow_AttachDummy* Dummy;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UTexture2D* CrossHair;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UTexture2D* Side_CrossHair;
private:
	void Seach_ComboIndex(EMouseBut but);

	virtual void Button_Event(EMouseBut input) override;
	void Montage_Play(FFPSWeaponData input);
	void Montage_Play(FFPSWeaponData input, FName Section);
private:
	FFPSWeaponData LastDatas; // ���ݿ� �¾��� ��� Hit Stop �� �����Ϸ��� index ���� ����
	FTransform Effect_Trans;	// ����Ʈ ��ġ �ȸ��� ��츦 ����
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FFPSWeaponData ComboData;	// ������ Combo Data �̰ɷ� Montage ���
private:
	bool R_push_Chk = false;
	bool L_push_Chk = false;

	

};
