// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Inven/CUserWidget_Slot.h"

#include "CCombatData.h"

#include "CWidget_EquipSlot.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCWidget_EquipSlot : public UCUserWidget_Slot
{
	GENERATED_BODY()
public:
	
	virtual void Slot_Swap(class UCUserWidget_Slot* swap_slot) override;
public:
	virtual void Slot_Use() override;
public:
	//override
	virtual void Equip_Item(class UCItem_Widget* item)override ;
	virtual void Equip(class UCItem_Widget* item) override;
	virtual void UnEquip() override ;
	
private:
	class UCItem_Widget* Item_Widget; //가지고있는 
	
};
