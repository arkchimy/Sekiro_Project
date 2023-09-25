// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CAttackments/CAttackment.h"
#include "CAttackment_Wizard.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API ACAttackment_Wizard : public ACAttackment
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	virtual void Enable_Combo() override;
	virtual void Unable_Combo() override;
	virtual void NextAttack_Play() override;

	virtual void Do_LAttack() override;
	virtual void Do_RAttack() override;
	virtual void L_AttackRelease() override;
	virtual void R_AttackRelease() override;

	virtual void Begin_Attack() override;
	virtual void End_Attack() override;

	UFUNCTION()
		virtual void OnBeginOverlap(const FHitResult& Hit, float power, AActor* Causer);
public:
	UFUNCTION()
		void ZoomAction();
	UFUNCTION()
		void PressedAction();
	UFUNCTION()
		void DrawDebug_Arc();
	virtual void Reload_Arraw() override;
private:
	float ZoomVal = 90;
	float PressVal = 0;
private:
	//TSubclassOf<class ACThrowItem> Throw_Class;
private:
	class ACThrowItem* causer;
	FEquipmentMontage Reload_Montage;
	class ACThrow_AttachDummy* Dummy;
};
