#pragma once

#include "CoreMinimal.h"
#include "CCombatData.h"

#include "Utilities/CEnums.h"  // Enum CSightEnum 사용

#include "Components/ActorComponent.h"

#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRTFOLIO_12_24_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

public:
	virtual void BeginPlay() override;
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		ECombat_Type Combat = ECombat_Type::Unarmed;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		ECombat_Type Swap_Combat = ECombat_Type::Unarmed;

public:
	void EquipSlot_Equip(int idx, UCCombatData* CombatData);
	void EquipSlot_UnEquip(int idx);

	void Play_Action(const EActionBut but);
	void Jump_End();
	void ChangeType(ECombat_Type type);
	void FPS_Weapon();
	void TPS_Weapon();
	FORCEINLINE ECombat_Type Get_CurrentCombat() { return Combat; }
	FORCEINLINE ECombat_Type Get_SwapCombat() { return Swap_Combat; }
	FORCEINLINE void Set_SwapCombat(ECombat_Type type) {  Swap_Combat = type; }
	FORCEINLINE UCCombatData* Get_Combat() { return Datas[int32(Combat)]; }
	
	ECamera_Sight Get_CombatSight(ECombat_Type type);
	FORCEINLINE ECamera_Sight Get_Sight() { return Datas[int32(Combat)]->Get_Sight(); }

protected:
	/*UPROPERTY(EditAnywhere)
		UCCombatData* Tempdata[int32(ECombat_Type::Max)];*/
	UPROPERTY(EditAnywhere)
		UCCombatData* Datas[int32(ECombat_Type::Max)];
	UPROPERTY(EditAnywhere)
		UCCombatData* UnArmed_Combat;
public:
	// =====   Swap var  =====
	bool Swapping = false;
	
	//            === Equipment === 
	void Equip();
	void Begin_Equip();
	void End_Equip();

	void UnEquip();
	void Begin_UnEquip();
	void End_UnEquip();

	//            === Weaponment ===  
	void On_Collision(EAttackTag_Type AttackType = EAttackTag_Type::AirBone, float Knoback_Power = 0.0f);
	void Off_Collision();

	void On_Parrying(); // Weapon is ObjectType Change ParryingMode
	void Off_Parrying();// Weapon is ObjectType Change StaticWeapon
	void ParryingMontage(EDirection direction); // Attacker
	void BeParriedMontage(EDirection direction,class ACharacter* ActionActor);// Hitted
	class UAnimMontage* Get_BeParriedMontage(EDirection direction);

	//           ===  Attackment   === 

	void Monster_Attack();	// Enemy Attack
	void Do_Parrying();
	void SuddenAttack();


	void L_Attack();
	void L_AttackRelease();

	void R_Attack();
	void R_AttackRelease();

	void T_Attack();
	void T_AttackRelease();

	void Begin_Attack();
	void End_Attack();

	void Enable_Combo();
	void Unable_Combo() ;
	void NextAttack_Play() ;
	void Resiste_PlayMontage();

	

	virtual void Reload_Arraw();// Arrow 
	virtual void Pressed_But(); // 버튼 Pressed 체크

	void Player_AttackSound();
	void Slot_Swap(class ACWeaponment* Weaponment);

public:
	class ACAttackment* Get_ACAttackment(){return attackment;}
private:
	class ACEquipment* equipment;
	class ACWeaponment* Weaponment;
	class ACAttackment* attackment;

	ACharacter* OwnerCharacter;



};
