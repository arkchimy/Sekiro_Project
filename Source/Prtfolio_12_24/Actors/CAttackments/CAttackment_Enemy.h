#pragma once

#include "CoreMinimal.h"
#include "Actors/CAttackments/CAttackment.h"
#include "CAttackment_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API ACAttackment_Enemy : public ACAttackment
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaTime) override;
public:
	UFUNCTION() // SetTimer  ÇÏ·Á°í
		virtual void Do_LAttack() override;
	virtual void Begin_Attack() override;
	virtual void End_Attack() override;
	virtual void Resiste_PlayMontage()override;
	
	virtual void TakeOverlap_Actor(const FHitResult& Hit, FVector Hit_location, AActor* Causer, EAttackTag_Type AttackTag, float KnockBack_Power)  override;

private:
	FAttackMontage LastDatas;
	FTransform Effect_Trans;
};
