// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndAttack.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCAnimNotify_EndAttack : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName() const;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	class ACharacter* OwnerCharacter;
	
	class UCombatComponent* Combatcomp;
};
