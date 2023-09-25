#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Utilities/CEnums.h" 
#include "Utilities/CStructs.h" 
#include "DataTable/FHitData.h"

#include "CStateComponent.generated.h"


USTRUCT(BlueprintType)
struct FAnim_Montage
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		bool bCanMove = false;
	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USoundCue* Hit_Sound;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRTFOLIO_12_24_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;
//
//public:	
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	FORCEINLINE EStateType Get_State() { return state; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsIdleState() { return state == EStateType::Idle; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsActionState() { return state == EStateType::Action; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsStunState() { return state == EStateType::Stun; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsFlyState() { return state == EStateType::Fly; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsHitState() { return state == EStateType::Hit; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsDeathState() { return state == EStateType::Death; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsEquipState() { return state == EStateType::Equip; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsBlockState() { return state == EStateType::Block; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsInteractionState() { return state == EStateType::Interaction; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsAbsolutState() { return state == EStateType::Absolut; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool IsRush() { return bRush; }

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UDataTable* DT_WeekHitData;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UDataTable* DT_HitData;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<FName> WeekBone;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAnim_Montage Attached_Montage; // duration Damage Hit Montage
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAnim_Montage DeathMontage;		
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FAnim_Montage FacedMontage;	// Enemy Faced Montage

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EStateType state;
public:
	UFUNCTION(BlueprintCallable)
		void Set_IdleMode();
	UFUNCTION(BlueprintCallable)
		void Set_ActionMode();
	UFUNCTION(BlueprintCallable)
		void Set_StunMode();
	UFUNCTION(BlueprintCallable)
		void Set_HitMode();
	UFUNCTION(BlueprintCallable)
		void Set_DeathMode();
	UFUNCTION(BlueprintCallable)
		void Set_EquipMode();
	UFUNCTION(BlueprintCallable)
		void Set_BlockMode();
	UFUNCTION(BlueprintCallable)
		void Set_InteractionMode();
	UFUNCTION(BlueprintCallable)
		void Set_AbsolutMode();

	
	UFUNCTION(BlueprintCallable)
		bool CanMove();
	FORCEINLINE void Set_Move() { bcanMove = true; }
	FORCEINLINE void Set_UnMove() { bcanMove = false; }
	FORCEINLINE void Set_HitBone(FName inval) { Hit_Bone = inval; }
	FORCEINLINE void Set_Rush(bool val) { bRush = val; }

public:
	void Hiteed_Direction(AActor* Causer , EDirection* direction);
	void Hitted_PlayMontage(AActor* causer, EAttackTag_Type attack_tag);
	void Play_Montage(const EDirection direction, const FName BoneName); // WeekHitted 일경우
	void Play_Montage(const EDirection direction,const EAttackTag_Type attack_tag); // || 아닐경우
	void Play_FacedMontage() ;


	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bcanMove = true;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		bool bRush = false;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FName> DataTable_Row;
	
private:
	FWeekHit_Data WeekHitData;
	FHit_Data HitData;


	class ACharacter* OwnerCharacter;
	FName Hit_Bone = FName("None");
	class ACEnemy* enemy;
};
