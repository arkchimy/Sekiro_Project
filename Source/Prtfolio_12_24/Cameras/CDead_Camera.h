// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDead_Camera.generated.h"

UCLASS()
class PRTFOLIO_12_24_API ACDead_Camera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACDead_Camera();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USceneComponent* Root;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USpringArmComponent* Spring;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCameraComponent* Camera;
	UFUNCTION(BlueprintImplementableEvent)
		void View_DeadCamera();


public:
	void Set_OwnerCharacter(class ACharacter* inOwner);
	void Set_View_Dead_Camera();
	
private:
	class ACharacter* OwnerCharacter;
	class ACPlayer* player;

};
