#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Utilities/CEnums.h"
#include "Utilities/CStructs.h"

#include "Actors/CThrowActors/CThrow_AttachDummy.h"

#include "GenericTeamAgentInterface.h" // Detection by Affiliation

#include "GameActor.generated.h"

UCLASS()
class PRTFOLIO_12_24_API AGameActor : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	
	AGameActor();
	//SetActorRelativeLocation(FVector::ZeroVector);
protected:
	
	virtual void BeginPlay() ;

public:	
	virtual void Tick(float DeltaTime) ;

	void GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {};
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) { return 0.0f; }
	virtual void Open_TreasureBox(TArray<FInven_Item> Treasureitems, class ACTreasureBox_Master* box) {};

	virtual void Close_TreasureBox() {};

	virtual void View_EnemyUI(class AGameActor* target) {};
public:
	ECombat_Type Get_Combat();
	class UParticleSystemComponent* Get_AttachParticle();
protected:
	/*
	* ==================  Editor
	*/
	
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool knockback = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class USoundCue* hit_Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 C_ID = 0;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Inventory")
		TArray<FInven_Item> inven_item;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class ACThrowItem> ThrowClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class ACThrow_AttachDummy> ThrowDummy_class;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class ACThrow_ManagerActor> ThrowManager_class;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UAnimInstance> Mesh_animInstance;
	/*
	* ==================  Component
	*/
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCameraComponent* Camera;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UBoxComponent* ParryingBoxComp; // 캐릭터와 캐릭터 간격 유지 

	/*
	* ==================  ActorComponent
	*/
protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCStateComponent* State;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Particle") // 속성 공격 당했을때 Particle Comp
		class UParticleSystemComponent* Attach_Particle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UCombatComponent* Combat_component;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)			// Particle 을 저장하는 ActorComp
		class UCActorComponent_Particle* Particle_ActorComp;
public:
	void KnockBack_AddOffset(AActor* Attacker);
	/* Target_Rotation Direction */
	UFUNCTION(BlueprintCallable)
		FRotator LookAt_Target();
	UFUNCTION(BlueprintCallable)
		FRotator ReverseLookAt_Target();
public:
	FORCEINLINE void Set_SkeletalMesh(USkeletalMesh* mesh) { GetMesh()->SetSkeletalMesh(mesh); }
	FORCEINLINE void Set_Attack_Tag(EAttackTag_Type tag) { Attack_tag = tag; }
	FORCEINLINE EAttackTag_Type Get_Attack_Tag() { return Attack_tag; }

	FORCEINLINE void Set_ParryingDirection(EDirection val) { Parrying_Direction = val; } // Parrying Direction Setter
	FORCEINLINE EDirection Get_ParryingDirection() { return Parrying_Direction; } // Parrying Direction Setter

	FORCEINLINE void Set_BeParriedDirection(EDirection val) { BeParried_Direction = val; } // BeParried Direction Setter
	FORCEINLINE EDirection Get_BeParriedDirection() { return BeParried_Direction; } // BeParried Direction Setter
	virtual FGenericTeamId GetGenericTeamId()const  override { return TeamID; }

	UFUNCTION(BlueprintCallable)
		virtual class ACThrow_AttachDummy* Get_ThrowDummy();
	FORCEINLINE class ACThrow_ManagerActor* Get_ThrowManager() { return Throw_Manager; }

	void TakeOverlap_SetAttackTag(EAttackTag_Type AttackTag, float power);
	FORCEINLINE void Set_HittedEffect(FHittedEffect data) {effect = data;}


protected:
	EAttackTag_Type Attack_tag = EAttackTag_Type::Normal; // 현재 Enemy의 상태 
	FHittedEffect effect;
public:
	/*
	* Inventory
	*/
	void Add_Item(int id);
	void Remove_Item(int id);
	TArray<FInven_Item> Has_Item();
	/* Player 전용 */
	virtual void OnFist() {};
	virtual void OnSS() {};
	virtual void OnArcher() {};
	virtual void OnWizard() {};

	virtual bool Target_Dot() { return false; }
	virtual class AGameActor* Get_TargetActor() { return Target_Actor; }
	virtual void Set_TargetActor(AGameActor* target) { Target_Actor = target; }

	EDirection TargetDirection(AGameActor* Target);
	void ParryingMontage(EDirection direction);
	void BeParriedMontage(EDirection direction, class ACharacter* ActionActor);
	
	void Counterable(bool val); // ParryingBoxComp OverlapEvent On/Off

	FORCEINLINE EKeybord Get_LastKey() { return LastKey; }
	/* Enemy*/
	FRotator hit_roation;
protected:
	FGenericTeamId TeamID;
	EKeybord LastKey; // 쉬프트나 스페이스바 누를 때 방향 저장용
						// 마지막으로 누른키를 저장 함
protected:
	class ACThrow_AttachDummy* Throw_Dummy; // StaticMesh 만 있음.
	class ACThrow_ManagerActor* Throw_Manager;

	EAttackTag_Type Last_AttackTag = EAttackTag_Type::AirBone;

	EDirection Parrying_Direction = EDirection::Max;
	EDirection BeParried_Direction = EDirection::Max;

	float Last_Knockback =0.0f;

	class AGameActor* Target_Actor = nullptr;
};
