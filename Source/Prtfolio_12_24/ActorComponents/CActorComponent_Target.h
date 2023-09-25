// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActorComponent_Target.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRTFOLIO_12_24_API UCActorComponent_Target : public UActorComponent
{
	GENERATED_BODY()

public:

	UCActorComponent_Target();
protected:
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Particle;
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Find_Target();
	void Skill_Look_Start();
	void Skill_Look_End();
	
	FORCEINLINE class AGameActor* Get_TargetActor() {return Target_Actor;}

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float TraceRadius = 3100; // Trace 할 원이 반지름
	UPROPERTY(EditAnywhere)
		float InterpSpeed = 5.5f;

protected:
	void Start_Targeting();
	void End_Targeting();
protected:
	class ACharacter* OwnerCharacter;
	class ACPlayer* player;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class AGameActor* Target_Actor;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class AGameActor* Owner_Actor;
public:
	FORCEINLINE bool Is_TargetMode() { return bTarget_Mode; }
	FORCEINLINE bool Target_Dot() { return bBackPos; }
private:
	bool bTarget_Mode = false; // Target Mode 중인지 체크
	bool bSkiil_Mode = false;
	bool bBackPos = false;
	class UParticleSystemComponent* Attached;
	
};
