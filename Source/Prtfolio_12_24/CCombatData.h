#pragma once

#include "CoreMinimal.h"
#include "Utilities/CEnums.h"  // Header에 Global을 추가하기 부담스러움
#include "Utilities/CStructs.h"  // Header에 Global을 추가하기 부담스러움
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"

#include "CCombatData.generated.h"




UCLASS()
class PRTFOLIO_12_24_API UCCombatData : public UDataAsset
{
	GENERATED_BODY()

public:
	void CombatData_Init(const UCCombatData& other);
public:
	virtual void BeginPlay(class ACharacter* InOwnerCharacter);
	virtual void DestroyActor();
public:
	FORCEINLINE class ACWeaponment* Get_Weaponment() { return Weaponment; }
	FORCEINLINE class ACEquipment* Get_Equipment();
	FORCEINLINE class ACAttackment* Get_Attackment() { return Attackment; }

	
	UFUNCTION()
		void SendCrossHair();
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ECamera_Sight Sight = ECamera_Sight::FPS; // Only Player

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UDataTable* Action_Table;				// Only Player
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UDataTable* ParryingTable;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UDataTable* BeParried_Table;
protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class ACEquipment> EquipmentClass;	 //  장착 ment 기능
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class ACWeaponment> WeaponmentClass; // Weapon 장비
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class ACAttackment> AttackTypeClass; // 어택 Montage


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)	// 화면 가운데 뜨는 마크 등록안하면 없음
		FCrossHairSet CrossHair;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UDataTable* Weapon_ComboTable;				//Weapon Combo 가 들어간 DATA Table 등록
	
public:
	void ParryingMontage(EDirection direction);
	void BeParriedMontage(EDirection direction, ACharacter* ActionActor);
	

	virtual void Play_Action(const EActionBut but) {};
	virtual void Jump_End() {};
	
	void Slot_Swap(ACWeaponment* SwapWeapon);//  인벤토리 실제 적용

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int Wheel_num = 1;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int Skill_Throw = 3;

public:
	FORCEINLINE virtual ECamera_Sight Get_Sight() { return ECamera_Sight::TPS; }
	FString GetLableName(class ACharacter* inOwner, FString name);
private:
	class ACWeaponment* Weaponment;
	class ACEquipment* Equipment;
	class ACAttackment* Attackment;

	TArray<FName> Parry_Data_size;
	TArray<FName> BeParryData_size;
	
protected:
	class ACharacter* OwnerCharacter;
public:
	void Set_OwnerCharacter(class ACharacter* data) { OwnerCharacter = data; }
};
