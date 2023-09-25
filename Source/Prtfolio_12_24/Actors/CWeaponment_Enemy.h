// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CWeaponment.h"

#include "CWeaponment_Enemy.generated.h"

/**
 * 
 */


UCLASS()
class PRTFOLIO_12_24_API ACWeaponment_Enemy : public ACWeaponment
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool NoWeapon = false;

public:
	virtual void On_Collision(EAttackTag_Type AttackType , float Knoback_Power) override;
	virtual void Off_Collision() override;

private:
	void Match_Collision(UShapeComponent* component);
	virtual void Box_Attach(UShapeComponent* component) override;
};
