#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CCombatData.h"
#include "CAnimInstance_Zombie.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCAnimInstance_Zombie : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//void BlueprintBeginPlay();
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation")
		float Speed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Animation")
		float Direction;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combat")
		ECombat_Type Combat;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Combat")
		bool Life = true;
private:
	class ACharacter* OwnerCharacter;
	class UCombatComponent* Combat_Component;
	class UCStateComponent* State_Component;
};
