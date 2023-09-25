// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Utilities/CEnums.h"
#include "CAnimNotifyState_Parrying.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCAnimNotifyState_Parrying : public UAnimNotifyState
{
	//¡÷ºÆ
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EDirection Parrying_Direction = EDirection::Max;
	virtual FString GetNotifyName() const;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	class AGameActor* OwnerCharacter;
	class UCombatComponent* CombatComp;
};

