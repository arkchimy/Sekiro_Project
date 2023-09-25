#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCombatData.h"
#include "CArmor.generated.h"

UCLASS()
class PRTFOLIO_12_24_API ACArmor : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Empty_Root")
		class USceneComponent* root;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Block_Montage")
		FEquipmentMontage Block_Montage;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Broken_Montage")
		FEquipmentMontage Broken_Montage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Particle")
		class UParticleSystemComponent* Attach_Particle;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USkeletalMeshComponent* TP_Mesh;
	/*UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USkeletalMeshComponent* FP_Mesh;*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCStateComponent_Armor* Armor_State;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Particle")
		class UParticleSystemComponent* Broken_Particle;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USoundCue* Broken_Cue;
public:	
	ACArmor();

protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintCallable)
		void On_Equip();
	UFUNCTION(BlueprintCallable)
		void On_UnEquip();

	
	UFUNCTION(BlueprintImplementableEvent)
		void Equip_Armor();
	UFUNCTION(BlueprintImplementableEvent)
		void UnEquip_Armor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Broken_Armor")
		void Armor_Broken();

	UFUNCTION(BlueprintCallable)
		void Armor_AttachToSocket(FName Socket);
	
	virtual void Broken_Event();
	
public:
	FORCEINLINE class UParticleSystemComponent* Get_AttachParticle() { return Attach_Particle; }
	virtual void Armor_TakeDamage(float Power,FString Attacker);// Causer가 공격 가능한 Tag를 가져옴.
	virtual void Armor_BlockMontage();
	UFUNCTION()
		virtual void Armor_BrokenMontage();
		
protected:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* state;
	class ACPlayer* player;
	class ACEnemy* Enemy;
	bool Broken_Armor = false;
	
private:
	
};
