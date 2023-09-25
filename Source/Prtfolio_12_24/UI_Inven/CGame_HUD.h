// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utilities/CStructs.h"
#include "CGame_HUD.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCGame_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void Open_Inventory();
	void Open_TreasureBox(TArray<FInven_Item> Treasureitems, class ACTreasureBox_Master* box);
	void Close_TreasureBox();

	void View_EnemyUI(class AGameActor* target);

	bool Item_PickUp(int id); // Slot 체크후 불가능하면 False
public:
	//BluePrint Callable
	UFUNCTION(BlueprintCallable)
		void Set_Inventory(class UCUserWidget_Inventory* inven, class APlayerController* Controller);
public:
	//implemnet
	UFUNCTION(BlueprintImplementableEvent)
		void EnemyUI_Init(class AGameActor* target); //Enemy Val Init Null 일경우 Hidden 
private:
	

	class UCUserWidget_Inventory* Inventory;
	
};
