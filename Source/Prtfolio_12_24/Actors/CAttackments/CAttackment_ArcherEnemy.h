#pragma once

#include "CoreMinimal.h"
#include "Actors/CAttackments/CAttackment.h"
#include "CAttackment_ArcherEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API ACAttackment_ArcherEnemy : public ACAttackment
{
	GENERATED_BODY()

public:
	virtual void Do_LAttack() override;
	virtual void Begin_Attack() override;
	virtual void End_Attack() override;


	virtual void L_AttackRelease() override;
	virtual void Reload_Arraw() override;

	UFUNCTION()
		virtual void OnBeginOverlap(const FHitResult& Hit, float power, AActor* Causer);

private:
	class ACThrowItem* causer;

	class ACThrow_AttachDummy* Dummy;
	float PressVal = 10;

private:
	FAttackMontage LastDatas;
	FTransform Effect_Trans;
};
