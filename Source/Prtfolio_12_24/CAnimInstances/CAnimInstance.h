#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"
#include "ActorComponents/CActorComponent_IKFeet.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void Init_Animinstance(); // OwnerCharacter가 초기화 안될경우가 발생해서 추가함. 
	//void BlueprintBeginPlay();
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation")
		float Speed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation")
		float Direction;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combat")
		ECombat_Type Combat;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FFeetData FeetData;

	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "But")
	//	bool LBut_Pressed = false;

	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "But")
	//	bool RBut_Pressed = false;
protected:
	class ACharacter* OwnerCharacter;

	class UCombatComponent* Combat_Component; 
	class UCStateComponent* State_Component;
	
	class UCActorComponent_IKFeet* FeetComp;
};
