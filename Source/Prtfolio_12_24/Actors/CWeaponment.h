#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/CArmor.h"
#include "Utilities/CEnums.h"  // Header에 Global을 추가하기 부담스러움

#include "CWeaponment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBeginOverlap,class ACharacter*, Attacker, class AActor*, causer, class ACharacter*, OtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEndOverlap, class ACharacter*, Attacker, class AActor*, causer, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FTakeDamageDelegate,const FHitResult&, Hit,FVector,Hit_location,AActor*,Causer,EAttackTag_Type,AttackTag,float,KnockBack_Power);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachDelegate);

//DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_FiveParams(FComponentHit_Weaponment, UPrimitiveComponent, OnComponentHit, UPrimitiveComponent*, HitComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, FVector, NormalImpulse, const FHitResult&, Hit);
USTRUCT(BlueprintType)
struct FHas_Armor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<ACArmor> Armors;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Socket_Name = FName("None");
};

UCLASS()
class PRTFOLIO_12_24_API ACWeaponment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACWeaponment();
	static class ACWeaponment* Factory_Weapon(UWorld* InWorld, TSubclassOf<class ACWeaponment>WeaponmentClass);
protected:
	virtual void BeginPlay() override;
public:
	virtual void BeginPlay(class ACharacter* InOwnerCharacter) { BeginPlay(); }
	virtual void Armor_Destroy();
private:
	// 무기 Overlap 관리
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnComponentHit_Attachmnet(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:									
	UFUNCTION(BlueprintCallable, Category = "Attach_Collision")
		void AttachToCollision(UShapeComponent* collision, FName Socket_name);
	UFUNCTION(BlueprintCallable, Category = "Attach_Actor")
		void AttachTo(FName Socket_name);
	virtual void Box_Attach(UShapeComponent* component) {}; // Enemy 전용으로 Box만 따로 Bone에 부착시킬떄.
	UFUNCTION(BlueprintCallable, Category = "Setter_Id")
		void Set_Id(int val) { id = val; }
	int Get_Id() { return id; }
public:
	UFUNCTION()
		void FPS_Weapon();
	UFUNCTION()
		void TPS_Weapon();

	UFUNCTION()
		void AttachToloop();
	UFUNCTION()
		void Weapon_Niagara(bool val);
	
	virtual void On_Collision(EAttackTag_Type AttackType, float Knoback_Power);
	virtual void Off_Collision();

	virtual void On_Parrying();
	virtual void Off_Parrying();

	void Item_PickUp();
	UFUNCTION()
		void Set_CollisionTag(TArray<ECollisionTags> collsiontags);


	FORCEINLINE void Set_OwnerCharacter(class ACharacter* inval) { OwnerCharacter = inval; }
	FORCEINLINE class ACharacter* Get_OwnerCharacter() { return OwnerCharacter; }
	void Destroy_Armor();
	
	bool IsPlayer(class ACharacter* InOwnerCharacter);

	FORCEINLINE class USkeletalMeshComponent* Get_Mesh() { return Weapon; }
	FORCEINLINE class USkeletalMeshComponent* Get_FpMesh() { return FP_Weapon; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE TArray<class ACArmor*> Get_armors() { return armor_Ref; }
public:
	bool IsFloorItem();
	
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UNiagaraComponent* Niagara;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USceneComponent* Scene;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USkeletalMeshComponent* Weapon;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UStaticMeshComponent* WeaponStatic;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USkeletalMeshComponent* FP_Weapon;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<FHas_Armor> has_Armor;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UParticleSystemComponent* Block_EffectComp;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UParticleSystemComponent* Wood_EffectComp;


	
	UPROPERTY(EditDefaultsOnly, Category = "SwordBlock_Sound")
		class USoundCue* SwordBlock_SoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "ShieldBlock_Sound")
		class USoundCue* SteelBlock_SoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "ShieldBlock_Sound")
		class USoundCue* WoodBlock_SoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "ShieldBlock_Sound")
		class USoundCue* Hit_SoundCue;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<FName> Block_Tag; // 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Broken_Power")
		float Brake_Power = 1.0f;


public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FName Holster = "Right_Hand";
protected:
	// 충돌 박스 
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<class UShapeComponent*> ShapeComponent; // 모든 충돌 박스

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		TArray<class AActor*> Overlap_Array;
	
public:
	FOnBeginOverlap On_Weaponment_BeginOverlap;
	FOnEndOverlap On_Weaponment_EndOverlap;
	
	FTakeDamageDelegate TakeDamage_Delegate;
	FAttachDelegate AttachTo_Delegate;
private:
	
	class UCStateComponent* State;
	class UCStatusComponent* Status;

	
	TArray<class ACArmor*> armor_Ref; //Armor Refer Save

protected:
	TArray<ECollisionTags> collison_tags;
	FName attachSocket;
	EAttackTag_Type AttackTag_Type;
	float Knoback_Power = 0.0f;
	class ACharacter* OwnerCharacter;
	class AGameActor* GameActor;
	class ACPlayer* player;
	class ACEnemy* enemy;
	
	bool bParrying = false;

	bool bOnFloor = false; // 버려진 아이템인지 체크
	int id = -1;

};
