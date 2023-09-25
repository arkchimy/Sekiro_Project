#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Utilities/CEnums.h"
#include "CAnimNotifyState_Collision.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCAnimNotifyState_Collision : public UAnimNotifyState
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EAttackTag_Type AttackType; // HitMontage 를 판별하기 위함
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float KnockBack_Power = 0.0f; // - 

	virtual FString GetNotifyName() const;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
	
	class AGameActor* OwnerActor;

	class ACPlayer* player;
	class ACEnemy* enemy;
	class UCombatComponent* CombatComp;
};
//virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime);