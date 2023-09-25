#include "CWidget_EquipSlot.h"
#include "Global.h"
#include "CUserWidget_Inventory.h"
#include "UI_Inven/CItem_Widget.h"

#include "ActorComponents/Combat_Component/CombatComponent.h"





void UCWidget_EquipSlot::Slot_Swap(UCUserWidget_Slot* swap_slot)
{
	
}

void UCWidget_EquipSlot::Slot_Use()
{
	CheckTrue(Get_info()->SlotType == ESlotType::Max);
	Inventory->EquipSlot_Use(this);
}

void UCWidget_EquipSlot::Equip_Item(UCItem_Widget* item)
{
	Item_Widget = item;
	FSlot_Info item_info = item->Get_Info();
		
	UCombatComponent* CombatComp = CHelpers::GetComponent<UCombatComponent>(Inventory->Get_Owner());
	CombatComp->EquipSlot_Equip(int32(item_info.SlotType), item_info.Attackment_class);
	return;
	
	
}
void UCWidget_EquipSlot::Equip(UCItem_Widget* item)
{
	CheckFalse(Possible_Check(item));
	Item_Widget = item;
	Insert_Item(item);
	
	FSlot_Info item_info = item->Get_Info();

	UCombatComponent* CombatComp = CHelpers::GetComponent<UCombatComponent>(Inventory->Get_Owner());
	CombatComp->EquipSlot_Equip(int32(item_info.SlotType), item_info.Attackment_class);
	Inventory->All_Slot_Hit(true);
	return;
}
void UCWidget_EquipSlot::UnEquip()
{
	CheckNull(Item_Widget);
	
	FSlot_Info item_info = Item_Widget->Get_Info();
	UCombatComponent* CombatComp = CHelpers::GetComponent<UCombatComponent>(Inventory->Get_Owner());
	CombatComp->EquipSlot_UnEquip(int32(item_info.SlotType));

	Inventory->All_Slot_Hit(true);
	Item_Widget = nullptr;
	return;
}


