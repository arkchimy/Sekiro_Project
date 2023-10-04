// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CEnemyLookAt_Target.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCEnemyLookAt_Target : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName() const;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	class ACharacter* OwnerCharacter;

	class UCStateComponent* State;
};
