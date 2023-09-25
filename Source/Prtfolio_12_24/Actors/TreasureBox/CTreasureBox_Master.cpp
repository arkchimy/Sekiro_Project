// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TreasureBox/CTreasureBox_Master.h"
#include "Global.h"
#include "Components/WidgetComponent.h"

#include "Actors/GameActor.h"
#include "Components/BoxComponent.h"


// Sets default values
ACTreasureBox_Master::ACTreasureBox_Master()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "RootCompo");
	CHelpers::CreateComponent<USceneComponent>(this, &Base_Scene, "Base_Scene", Root);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Base, "TreasureBox", Base_Scene);

	CHelpers::CreateComponent<USceneComponent>(this, &Cap_Scene, "Cap_Scene", Root);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Cap, "Cap", Cap_Scene);

	CHelpers::CreateComponent<UWidgetComponent>(this, &Hint_UI, "Hint_UI", Base_Scene);
	
	CHelpers::CreateComponent<UBoxComponent>(this, &CollisionMesh, "Collision", Base_Scene);

	RootComponent = Root;
	CollisionMesh->SetRelativeLocation(FVector(0, 0, 0));
}

void ACTreasureBox_Master::Open_TreasureBox()
{
	if (!bOpen) 
	{
		bOpen = true;
		
		UKismetSystemLibrary::K2_ClearTimer(this, "Close_Animation");
		Open_Animation();
		if (!!TouchActor)
		{
			TouchActor->Open_TreasureBox(Has_Item,this);
		}
		return;
	}
	
	
	bOpen = false;
	if (!!TouchActor)
	{
		TouchActor->Close_TreasureBox();
	}
	Close_TreasureBox();

}
void ACTreasureBox_Master::Open_Animation()
{
	FRotator rotator = FRotator(90, 0, 0);
	FRotator current = Cap_Scene->GetRelativeRotation();

	current = UKismetMathLibrary::RInterpTo(current, FRotator(rotator.Pitch, current.Yaw, current.Roll), 0.02f, OpenSpeed);
	Cap_Scene->SetRelativeRotation(current);
	UKismetSystemLibrary::K2_SetTimer(this, "Open_Animation", 0.02f, false);
	if (89 <= Cap_Scene->GetRelativeRotation().Pitch)
	{
		CLog::Print(Cap_Scene->GetRelativeRotation().Pitch);
		UKismetSystemLibrary::K2_ClearTimer(this, "Open_Animation");

	}
}

void ACTreasureBox_Master::Close_TreasureBox()
{
	UKismetSystemLibrary::K2_ClearTimer(this, "Open_Animation");
	Close_Animation();
	TouchActor = nullptr;
}

void ACTreasureBox_Master::Close_Animation()
{
	FRotator rotator = FRotator(0, 0, 0);
	FRotator current = Cap_Scene->GetRelativeRotation();

	current = UKismetMathLibrary::RInterpTo(current, FRotator(rotator.Pitch, current.Yaw, current.Roll), 0.02f, OpenSpeed);
	Cap_Scene->SetRelativeRotation(current);
	UKismetSystemLibrary::K2_SetTimer(this, "Close_Animation", 0.02f, false);
	if (0.1f >= Cap_Scene->GetRelativeRotation().Pitch)
	{

		UKismetSystemLibrary::K2_ClearTimer(this, "Close_Animation");

	}
}

// Called when the game starts or when spawned
void ACTreasureBox_Master::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ACTreasureBox_Master::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACTreasureBox_Master::Set_TouchActor(AGameActor* actor)
{
	TouchActor = actor;
}

void ACTreasureBox_Master::Add_Item(int id)
{

	CheckNull(TouchActor); //Touch Actor Abort
	TouchActor->Remove_Item(id);

	FInven_Item newitem;
	newitem.id = id;
	Has_Item.Add(newitem);

}

void ACTreasureBox_Master::Remove_Item(int id)
{
	CheckNull(TouchActor); //Touch Actor Abort

	int i = 0;
	for (i = 0; i < Has_Item.Num(); i++)
	{
		if (Has_Item[i].id == id)
		{	// ID와 일치하는 idx 찾음
			Has_Item.RemoveAt(i);
			break;
		}
	}
	TouchActor->Add_Item(id);
}

void ACTreasureBox_Master::Throw_TreasureItem(int id)
{
	int i = 0;
	for (i = 0; i < Has_Item.Num(); i++)
	{
		if (Has_Item[i].id == id)
		{	// ID와 일치하는 idx 찾음
			Has_Item.RemoveAt(i);
			break;
		}
	}
}

void ACTreasureBox_Master::Set_OwnerActor(AGameActor* actor)
{
	OwnerActor = actor;
}

void ACTreasureBox_Master::Treasure_UI_Off()
{
	
	if (!!TouchActor)
	{
		TouchActor->Close_TreasureBox();
		Close_TreasureBox();
	
		
	}
}
