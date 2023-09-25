// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_DonMove.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCAnimNotifyState_DonMove : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName() const;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	class ACEnemy* enemy;
	class ACAIController_Enemy* controller;
};
