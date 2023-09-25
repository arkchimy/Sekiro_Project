#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCombatData.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRTFOLIO_12_24_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Walk_Speed = 150;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Run_Speed = 400;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Fly_Speed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Max_Hp = 50;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Max_Mp  = 0 ;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float hp = 50;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float mp = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<FEquipmentMontage> Hited_Montage;
public:
	FORCEINLINE float Get_WalkSpeed() { return Walk_Speed; }
	FORCEINLINE float Get_RunSpeed() { return Run_Speed; }
	FORCEINLINE float Get_FlySpeed() { return Fly_Speed; }

public:
	UFUNCTION(BlueprintPure, BlueprintCallable)
		float GetHp() { return hp; }
	UFUNCTION(BlueprintCallable)
		void Sub_Health(float inVal);
	FORCEINLINE float GetMp() { return mp; }

public:
	FORCEINLINE void SetMove() { bCanMove = true; }
	FORCEINLINE void SetStop() { bCanMove = false; }

public:
	void Add_Health(float inVal);

	void Add_Mana(float inVal);
	void Sub_Mana(float inVal);

private:

	bool bCanMove = true;
	class ACharacter* OwnerCharacter;
};
