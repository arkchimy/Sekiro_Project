#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actors/GameActor.h"

#include "CEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeadDelgate);

UCLASS()
class PRTFOLIO_12_24_API ACEnemy : public AGameActor
{
	GENERATED_BODY()

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:			//  --- Delegate ---
	FDeadDelgate DeadDelegate;
	
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
public:
	void Set_HitBone(FName inval);
	void AttachToCollision(class AActor* Weapon, FName SocketName);
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Cine")
		bool bHasCine = false;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UBehaviorTree* BT_Asset;

	UFUNCTION(BlueprintCallable)
		virtual void OnFaceMontage();
	UFUNCTION(BlueprintCallable)
		virtual void OnDeathMontage() {};
	UFUNCTION(BlueprintCallable)
		void Notify_TakeDamage(float Damage);
	UFUNCTION(BlueprintCallable)
		void DeathMode();


	UFUNCTION(BlueprintCallable) // Throw Monster 일 때 발사하는 Notify
		void Throw_Action_Notify();

	void Resiste_PlayMontage();


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void Broken_Mesh();

	void OnFist();
	void OnSS();
	void OnArcher();
	void OnWizard();
	UFUNCTION(BlueprintCallable)
		void Monster_Attack();
	UFUNCTION(BlueprintCallable)
		class UAnimMontage* Get_NextMontage();

	FORCEINLINE void Set_Interactive(bool inval) { bInteraction = inval; }
	FORCEINLINE bool Get_Interactive() { return bInteraction; }

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bInteraction = false;
	
	

public:

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)  // 특정 공격시 Player의 Camera를 흔드는 역할.
		TSubclassOf<class ACCamera_Manager> CameraManager_Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)  // 얼음 공격에 당했을때 죽으면 부서지기
		bool Broken_Possible = false;

public:
	FORCEINLINE bool GetLife_Chk() { return Life_Chk; }
	bool Life_Chk = true;

	int Throw_ID = 1;//attach Arrow;
private:
	UPROPERTY(EditAnywhere)
		class ACCamera_Manager* Camara_Manager;
protected:
	class ACAIController_Enemy* controller;

private:
	void Weapon_Change(ECombat_Type type);
	class ACPlayer* player;
	class ACPlayer* cine_player;
	bool bOnce = true;
};
