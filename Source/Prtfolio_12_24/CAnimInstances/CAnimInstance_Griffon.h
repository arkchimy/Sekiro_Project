#pragma once

#include "CoreMinimal.h"
#include "CAnimInstance.h"
#include "ActorComponents/State_Component/CStateComponent.h"
#include "CAnimInstance_Griffon.generated.h"


UCLASS()
class PRTFOLIO_12_24_API UCAnimInstance_Griffon : public UCAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//void BlueprintBeginPlay();
protected:
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation")
	//	float Speed;
	//UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation")
	//	float Direction;
	/*UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combat")
		ECombat_Type Combat;*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "State")
		EStateType State;

};
