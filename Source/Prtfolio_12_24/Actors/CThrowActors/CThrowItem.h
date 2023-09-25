#pragma once

#include "CoreMinimal.h"
#include "CCombatData.h"
#include "Actors/Builes/CBuild.h"
#include "GameFramework/Actor.h"
#include "CThrowItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FThrowBeginOverlap, const FHitResult&, InHitResult,float,Power,AActor* ,Causer);

USTRUCT(BlueprintType)
struct FThrow_Edit // 오브젝트에 따른 변동 edit값
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FRotator Rot = FRotator(0,0,0);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector Scale = FVector(1, 1, 1);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FVector Loc = FVector(0, 0, 10);
};

USTRUCT(BlueprintType)
struct FTag_CollisionSound
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<FName> Tags;
	UPROPERTY(EditDefaultsOnly, Category = "Hit_Sound")
		class USoundCue* Hit_SoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "Hit_Sound")
		class USoundCue* Critical_SoundCue;
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "Hit_Sound")
		class UParticleSystem* Impact_particle;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hit_Sound")
		FTransform ImpactTrans;
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly, Category = "Hit_Sound")
		class UParticleSystem* Blood_particle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hit_Sound")
		FTransform BloodTrans;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Hit_Sound")
		FVector Blood_Scale = FVector(1,1,1);
};


UCLASS()
class PRTFOLIO_12_24_API ACThrowItem : public AActor
{
	GENERATED_BODY()
public:
	ACThrowItem();
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<FParticle> Particle;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UParticleSystemComponent* Particle_Comp;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USoundCue* Preesed_SoundCue;
	UPROPERTY(EditDefaultsOnly, Category = "Hit_Sound")
		class USoundCue* Wood_SoundCue;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FTag_CollisionSound Tag_Hit;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FTag_CollisionSound Tag_Attach;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FTag_CollisionSound Tag_Block;
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float Power = 3.0f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FName Socket_Name = "Grip_Socket";
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FEquipmentMontage ReloadMontage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float Max_Inval = 60.0f;		//Pressed로 늘어날수있는 최댓값
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float Impulse_Power = 100.0f;

	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	UFUNCTION()
		virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void OnThrowComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	FThrowBeginOverlap ThrowComponentHit;

	FORCEINLINE class UStaticMesh* Get_ThrowMesh() { return Mesh->GetStaticMesh(); }
	FORCEINLINE class UStaticMeshComponent* Get_Mesh() { return Mesh; }
	FORCEINLINE FEquipmentMontage Get_ReloadMontage() { return ReloadMontage; }
public:
	FORCEINLINE class ACharacter* Get_OwnerCharacter() { return OwnerCharacter; }
	FORCEINLINE void Set_Throwing(bool inval) { Throwing = inval; }
	FORCEINLINE bool Get_AttachChk() { return Attachk_chk; }
	FORCEINLINE float Get_ImpulsePower() { return Impulse_Power; };

	void Set_Visible(bool inval);
	bool Get_Throwing();
	
	bool Get_Visible();

public:

	UFUNCTION(BlueprintCallable)
		virtual void Set_Pressed(float inval, ACharacter* OwnerCharacter);
	UFUNCTION()
		void Reset_Visible();
	UFUNCTION(BlueprintImplementableEvent)
		void Trail_Visible(bool val);
	UFUNCTION(BlueprintCallable)
		void Timer_Boom();
	UFUNCTION(BlueprintImplementableEvent, Category = "TimerBoom")
		void Timer_Reset();
protected:
	virtual void Draw_Route(FVector Hit_location) {};
	UFUNCTION(BlueprintImplementableEvent)
		void Shoot(FVector velocity);


	FVector Create_Location;
protected:
	float Orizinal_Power;
	FVector Orizinal_Scale;
	bool bBlocked = false; // true 면 Overlap 안됌
	bool visible_val =  false;
	bool Attachk_chk = false;
	bool Throwing = false;

public:
	FORCEINLINE int Get_Id() { return id; }
	FORCEINLINE void Set_Id(int value) { id = value; }
	FORCEINLINE FName Particle_State() { return State; }
private:
	int id = 0;

	class ACThrow_ManagerActor* Manager;
public:
	void Set_Manager(ACThrow_ManagerActor* owner);
protected:
	bool Attach_Owner = false;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FThrow_Edit Edit; // Mesh마다 다른 Rot 보정 값

public:
	virtual void Set_State(FName val);
protected:
	FName State;
	class ACharacter* OwnerCharacter;

};
