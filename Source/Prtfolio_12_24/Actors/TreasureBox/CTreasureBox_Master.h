// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utilities/CStructs.h"

#include "CTreasureBox_Master.generated.h"

UCLASS()
class PRTFOLIO_12_24_API ACTreasureBox_Master : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USceneComponent* Root;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USceneComponent* Base_Scene;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UStaticMeshComponent* Base;	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USceneComponent* Cap_Scene;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UStaticMeshComponent* Cap;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UWidgetComponent* Hint_UI;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UBoxComponent* CollisionMesh;
	
public:	
	// Sets default values for this actor's properties
	ACTreasureBox_Master();
public:
	// SetTimer
	
	void Open_TreasureBox();
	UFUNCTION()
		void Open_Animation();
	UFUNCTION()
		void Close_Animation();
	void Close_TreasureBox();
public:
	// BluePrint
	UFUNCTION(BlueprintCallable)
		void Treasure_UI_Off();
public:
	//	Init
	void Set_TouchActor(class AGameActor* actor);
	void Set_OwnerActor(class AGameActor* actor);

	void Add_Item(int id);
	void Remove_Item(int id);
	void Throw_TreasureItem(int id);
private:
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	float OpenSpeed = 1.f; // Box Open Speed
	bool bOpen = false;	//Open Close Event ÆÇ´Ü Val
	class AGameActor* OwnerActor;
	UPROPERTY(BlueprintReadOnly)
		class AGameActor* TouchActor;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<FInven_Item> Has_Item;
	
};
