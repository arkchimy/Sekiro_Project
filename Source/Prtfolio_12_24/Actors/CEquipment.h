#pragma once

#include "CoreMinimal.h"
#include "CCombatData.h"
#include "GameFramework/Actor.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentDelegate,FName , SocketName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCrossHairDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnequipmentDelegate, FName, SocketName);

UCLASS()
class PRTFOLIO_12_24_API ACEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACEquipment();

protected:
	virtual void BeginPlay() override;
public:
	//장착 명령
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	//장착 개시
	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	void Begin_Equip_Implementation();

	//장착 완료
	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();


	//해제 명령
	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	void Unequip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Begin_UnEquip();
	void Begin_UnEquip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_UnEquip();
	void End_UnEquip_Implementation();

public:
	UPROPERTY(BlueprintAssignable)
		FEquipmentDelegate OnEquipmentDelegate;

	UPROPERTY(BlueprintAssignable)
		FUnequipmentDelegate OnUnequipmentDelegate;

	UPROPERTY(BlueprintAssignable)
		FCrossHairDelegate Cross_Hair;
	/*FORCEINLINE void Set_EquipMontage(FEquipmentMontage inval) { Equip_Montage = inval; }
	FORCEINLINE void Set_UnEquipMontage(FEquipmentMontage inval) { UnEquip_Montage = inval; }*/
	FORCEINLINE void Set_OwnerCharacter(class ACharacter* inval) { OwnerCharacter = inval; }
private:
	class ACharacter* OwnerCharacter;
	class USkeletalMeshComponent* FP_Mesh;
protected:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName Sheathe_Socket;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName Draw_Socket;
	
public:
	class UCStatusComponent* Status;
	class UCStateComponent* State;
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FEquipmentMontage Equip_Montage;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FEquipmentMontage UnEquip_Montage;
	
	class AGameActor* Owner;
	class ACPlayer* player;
	class ACEnemy* enemy;

	
};
