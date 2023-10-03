// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"


#include "Strategy_Pattern/Operation_Strategy.h"

#include "Utilities/CEnums.h"
#include "CAIController_Enemy.generated.h"

/**
 *  AIController - Type Melee _Archer _Boss Child. 
 */
UCLASS()
class PRTFOLIO_12_24_API ACAIController_Enemy : public AAIController
{
	GENERATED_BODY()
public:
	ACAIController_Enemy();
	~ACAIController_Enemy();
	virtual void BeginPlay() override;
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UAIPerceptionComponent* Perception;



	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class UOperation_Context> Context_Class;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		EOperation_Type Operation_Type = EOperation_Type::Patrol;
public:
	UFUNCTION(BlueprintCallable)
		void Random_Select_Operation();
	void Set_FocusActor(AActor* Attacker); // 다격하면 그사람 바라보기
	UFUNCTION(BlueprintCallable)
		bool interp_Focus(FVector targetlotation, float deltatime); // true = near zero  false not zero
protected:

	UFUNCTION()
		void OnTargetDetected(class AActor* Actor, FAIStimulus Stimulus);
	

	FGenericTeamId TeamID;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	virtual FGenericTeamId GetGenericTeamId()const  override { return TeamID; }

	void MoveUpdate(bool val);
	void Damaged_Target(class AActor* Attacker);

protected:
	virtual void OnPossess(APawn* InPawn) override;

	virtual void SetPerception();

protected:
	

	class UAISenseConfig_Sight* SightConfig;
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Perception")
		float AISightRadius = 900.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Perception")
		float AILoseSightRadius = 50.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Perception")
		float AIFieldOfView = 45.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Perception")
		float AISightAge = 5.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Perception")
		float AILastSeenLocation = 1200.f;
/*
	BlackBoard Function
*/
	UFUNCTION()
		void Update_Target();
	UFUNCTION(BlueprintCallable)
		void BlackBoard_Update(); // Operation & SetCanMove
	UFUNCTION(BlueprintCallable)
		void Monster_Dead(); // Operation & SetCanMove
	void Set_TeamId(int32 val) { TeamID = val; }
	
	UFUNCTION(BlueprintCallable)
		void Monster_Action(FVector Loc); // TargetActor Loc || MovePoint;
protected:
	FString Enemy_Type = "Melee";
	FTimerHandle target_handler;
	class UCStateComponent* State;
	class UOperation_Context* Context;
	IOperation_Strategy* oper;
};
