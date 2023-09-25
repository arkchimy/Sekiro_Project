#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCombatData.h"

#include "Utilities/CEnums.h"  // Header에 Global을 추가하기 부담스러움
#include "Utilities/CStructs.h"

#include "Engine/DataTable.h"

#include "CAttackment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackTag_Delegate,TArray<ECollisionTags>, colliontags); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponNiagaraDelegate,bool,Val); // 공격 발생시 Niagara On/Off



UCLASS()
class PRTFOLIO_12_24_API ACAttackment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAttackment();

protected:
	virtual void BeginPlay() override;
public:					//Delegate
	FWeaponNiagaraDelegate WeaponNiagara_Delegate;
public:
	
	FORCEINLINE void Set_OwnerCharacter(class ACharacter* inval) { OwnerCharacter = inval; }
	void Set_ComboTable(UDataTable* table);
public:
	FORCEINLINE void Set_ThrowItemID(int val) { ThrowItemID = val; }
	FORCEINLINE void Set_SkillThrowID(int val) { ThrowItem_Skill = val; }
public:

	virtual void Monster_Attack() {}; // Enemy Attack
	virtual class UAnimMontage* Get_NextMontage() { return nullptr; }

	virtual void Do_LAttack() {};
	virtual void Do_RAttack() {};
	virtual void Do_Parrying() {};	// 패링
	virtual void SuddenAttack() {}; // 암살

	virtual void L_AttackRelease() {};
	virtual void R_AttackRelease() {};
	virtual void Pressed_But() {};

	virtual void T_Attack() {};
	virtual void T_AttackRelease() {};
	virtual void R_Attack() {};
	

	virtual void Begin_Attack() {};
	virtual void End_Attack() {};

	virtual void Enable_Combo() {};
	virtual void Unable_Combo() {};
	virtual void NextAttack_Play() { };
	virtual void Resiste_PlayMontage() {};


	virtual void Button_Event(EMouseBut input) {}; // override 로 재정의 후 사용
	virtual void Player_AttackSound() {};
	UFUNCTION()
		virtual void TakeOverlap_Actor(const FHitResult& Hit, FVector Hit_location, AActor* Causer,EAttackTag_Type AttackTag,float KnockBack_Power) { }
	virtual void Reload_Arraw() {};
public:
	FAttackTag_Delegate AttackTag_Delegate;// 어떤 신체 부위 collision  Onoff
protected:

	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;
	class UCStatusComponent* Status;

	UPROPERTY(EditDefaultsOnly, Category = "AttackSound")
		class USoundCue* Attack_Sound;

protected:
	TArray<AActor*> Hited_Character;

	class AGameActor* OwnerActor;
	class ACPlayer* player;
	class ACEnemy* enemy;
	int Attack_Cnt = 0;
	
	bool Nextable = false;
	bool NextActtack = false;

	int ThrowItemID = 1;
	int ThrowItem_Skill = 3;
private:

public:
	FORCEINLINE void Set_CameraShake(TSubclassOf<UCameraShakeBase> inval) { shake = inval; };
protected:
	TSubclassOf<UCameraShakeBase> shake;
	UDataTable* ComboTable; // 자식 클래스에서 사용 가능. 

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FName> ComboRow;
	
};
