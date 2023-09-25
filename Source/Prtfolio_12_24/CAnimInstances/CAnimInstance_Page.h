// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActorComponents/Combat_Component/CPageCombatComponent.h"

#include "CAnimInstance_Page.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCAnimInstance_Page : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		EPageCombat combat = EPageCombat::PickUp;
private:
	class ACPageMaster* Page;
	class ACBuild_Book* Owner_Book;

protected:
	class UCPageCombatComponent* CombatComp;


};
