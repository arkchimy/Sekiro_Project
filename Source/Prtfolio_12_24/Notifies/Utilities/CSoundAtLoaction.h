// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CSoundAtLoaction.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCSoundAtLoaction : public UAnimNotify
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class USoundBase* Sound;
public:
	virtual FString GetNotifyName() const;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);
protected:
	
};
