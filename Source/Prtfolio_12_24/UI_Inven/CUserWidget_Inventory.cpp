#include "UI_Inven/CUserWidget_Inventory.h"
#include "global.h"
#include "Containers/Map.h"

#include "Actors/GameActor.h"
#include "Actors/TreasureBox/CTreasureBox_Master.h"

#include "GameFramework/PlayerController.h"



void UCUserWidget_Inventory::All_Slot_Hit(bool val)
{
	// All SlotWidget Hitalbe   Reset Init
	for (UCItem_Widget* widget : Item_widgets)
	{
		widget->Img_Hitable(val);
	}
	for (UCItem_Widget* widget : Treasure_Items)
	{
		widget->Img_Hitable(val);
	}
	
}



void UCUserWidget_Inventory::Init_MouseControl(APlayerController* Controller)
{
	Mouse = Controller;
	OwnerActor = Cast<AGameActor>(Mouse->GetPawn());
	Inventory_Update(OwnerActor->Has_Item());
}


void UCUserWidget_Inventory::Open_Inventory()
{
	
	if (bInven_Panel)
	{	// Close   Inven Panel이 켜져있다면.
		
		bInven_Panel = false; // flip  기능
		Mouse->SetShowMouseCursor(false);
		SlotHasItem_Visible(false, EPanelTarget::Inven);
		
		return;
	}
	else 
	{	//Open
		bInven_Panel = true;
		SlotHasItem_Visible(true, EPanelTarget::Inven);
		Mouse->SetShowMouseCursor(true);
		
	}
}
void UCUserWidget_Inventory::Open_TreasureBox()
{
	
	SlotHasItem_Visible(true, EPanelTarget::TreasureBox);
	Mouse->SetShowMouseCursor(true);

	for (UCItem_Widget* item_widget : Treasure_Items)
	{
		UCUserWidget_Slot* slot_widget = Find_Slot(Treasure_Slots, item_widget);
		if (!!slot_widget)
			slot_widget->Insert_Item(item_widget);
		else
			CLog::Print("Treasure is Full");
	}
	
}
void UCUserWidget_Inventory::Close_TreasureBox()
{
	
	
	SlotHasItem_Visible(false, EPanelTarget::TreasureBox);
	Treasure_Items.Empty();
	for (UCUserWidget_Slot* TreasureSlot : Treasure_Slots)
		TreasureSlot->Reset_LinkItemWidget(ESlotType::Max); // TreasureSlot만 reset
	Mouse->SetShowMouseCursor(false);
	
}
AActor* UCUserWidget_Inventory::Get_Owner()
{
	return Cast<AActor>(OwnerActor);
}
void UCUserWidget_Inventory::Inventory_Update(TArray<FInven_Item> has_items)
{
	// Inventory Init 용도

	TArray<FName> rows = DT_Item->GetRowNames();
	// find Id equal
	for (FInven_Item has_item : has_items)
	{
		item = *(DT_Item->FindRow<FSlot_Info>(rows[has_item.id], rows[has_item.id].ToString()));
		Create_ItemWidget(item); // 생성하고 Item_Widget에 BluePrint로 Add
	}
	if (Do_once)
	{	// 처음에 가진 아이템 그리드 정렬 하기  1회만 실행
		Do_once = false;

		for (UCItem_Widget* item_widget : Item_widgets)
		{
			
			UCUserWidget_Slot* slot_widget = Find_Slot(Inven_Slots, item_widget);
			if (!!slot_widget) 
			{
				slot_widget->Insert_Item(item_widget);
				
			}
			else
				CLog::Print("Inventory is Full");
		}
	}
}

void UCUserWidget_Inventory::TreasureBox_Update(TArray<FInven_Item> Treasures)
{
	for(UCItem_Widget* widget :Treasure_Items)
	{	// Itemwidget Hidden 을하고 메모리 반환
		Reset_ItemWidget(widget);
	}
	Treasure_Items.Empty();
	TArray<FName> rows = DT_Item->GetRowNames();
	// find Id equal
	for (FInven_Item Treasure : Treasures)
	{
		item = *(DT_Item->FindRow<FSlot_Info>(rows[Treasure.id], rows[Treasure.id].ToString()));
		Create_OtherWidget(item); //  BluePrint로 Treasure_Items Add 
	}
}

void UCUserWidget_Inventory::Treasure_Add(int id)
{
	if (!!TouchTreasure)
	{
		TouchTreasure->Add_Item(id);
	}
	else
		CLog::Print("TouchTreasure is Null");
}

void UCUserWidget_Inventory::Treasure_Remove(int id)
{
	if (!!TouchTreasure)
	{
		TouchTreasure->Remove_Item(id);
	}
	else
		CLog::Print("TouchTreasure is Null");
}

void UCUserWidget_Inventory::Throw_InvenItem(int id)
{
	OwnerActor->Remove_Item(id);
}

void UCUserWidget_Inventory::Throw_TreasureItem(int id)
{
	TouchTreasure->Throw_TreasureItem(id);
}

bool UCUserWidget_Inventory::Item_PickUp(int id)
{
	TArray<FName> rows = DT_Item->GetRowNames();
	// find Id equal
	item = *(DT_Item->FindRow<FSlot_Info>(rows[id], rows[id].ToString()));
	Create_TempWidget(item); //  Temp_Widget Init
	if(!!Temp_Widget)
	{
		UCUserWidget_Slot* slot_widget = Find_Slot(Inven_Slots, Temp_Widget);
		if (!!slot_widget)
		{	//빈공간 있을때
			slot_widget->Insert_Item(Temp_Widget);
			Item_widgets.Add(Temp_Widget);

			return true;
		}
		else 
		{	//공간 불충분
			CLog::Print("Inventory is Full");
			Reset_AllSlotTint();
			return false;
			
		}
	}
	return false;
}



void UCUserWidget_Inventory::Reset_ItemWidget(UCItem_Widget* widget)
{
	widget->SetVisibility(ESlateVisibility::Hidden);
	widget->~UCItem_Widget();
}

void UCUserWidget_Inventory::Reset_AllSlotTint()
{
	for (UCUserWidget_Slot* slot : Inven_Slots)
		slot->Reset_Tint();
	for (UCUserWidget_Slot* slot : Treasure_Slots)
		slot->Reset_Tint();

}

UCUserWidget_Slot* UCUserWidget_Inventory::Find_Slot(TArray<UCUserWidget_Slot*> slots , UCItem_Widget* Item_info)
{
	int num = 0 ;
	for (UCUserWidget_Slot* slot : slots)
	{
		num++;
		if (!slot->IsUsed_Slot())
		{	// 사용되지 않은 슬롯찾기
			if (slot->Possible_Check(Item_info))
			{	// 아이템 크기 체크
				return slot;
			}
		}
	}
	CLog::Print("Empty_Slot Not Found");
	return nullptr;
}

TPair<int32, int32> UCUserWidget_Inventory::Slot_Size(const FSlot_Info info)
{
	switch (info.SlotType)
	{
	case ESlotType::Head:
		return TPair<int32, int32>(2, 2);
	case ESlotType::Sheath_Weapon:
		return TPair<int32, int32>(1, 3);
	case ESlotType::Upper_Weapon:
		return TPair<int32, int32>(2, 4);
	case ESlotType::Body:
		return TPair<int32, int32>(2, 3);
	case ESlotType::QuickSlot:
		return TPair<int32, int32>(1, 2);
	case ESlotType::Max:
		return TPair<int32, int32>(0, 0);
	default:
		return TPair<int32, int32>(1, 1);
	}

}

void UCUserWidget_Inventory::SlotHasItem_Visible(bool val, EPanelTarget target)
{
	bInven_Panel = val;
	InvenPanel_Visible(val);
	if (target == EPanelTarget::Inven)
	{
		for (UCUserWidget_Slot* slots : Equip_Slots[0])
			slots->SlotHasItem_Visible(val);
		for (UCUserWidget_Slot* slots : Equip_Slots[1])
			slots->SlotHasItem_Visible(val);
		for (UCUserWidget_Slot* slots : Equip_Slots[2])
			slots->SlotHasItem_Visible(val);
		for (UCUserWidget_Slot* slots : Equip_Slots[3])
			slots->SlotHasItem_Visible(val);
		for (UCUserWidget_Slot* slots : Equip_Slots[4])
			slots->SlotHasItem_Visible(val);
		
		for (UCItem_Widget* widget : Treasure_Items)
		{
			widget->View_Item(val);
		}
		for (UCItem_Widget* widget : Item_widgets)
		{	// Update 에서 가져온 Create한 Widget들
			widget->View_Item(val);

		}
		for (UCUserWidget_Slot* slot_hasitem : Inven_Slots)
		{	//Dynamic 한 Widget들의 이동으로 인해 Slot 이 가지고있는 Widget들
			slot_hasitem->SlotHasItem_Visible(val);
		}
		
		
		
	}

	if (target == EPanelTarget::TreasureBox)
	{
		
		TreasurePanel_Visible(val);
		
		for (UCItem_Widget* widget : Treasure_Items)
		{
			widget->View_Item(val);
		}
		for (UCItem_Widget* widget : Item_widgets)
		{	// Update 에서 가져온 Create한 Widget들
			widget->View_Item(val);
		}
		for (UCUserWidget_Slot* slot_hasitem : Inven_Slots)
		{	//Dynamic 한 Widget들의 이동으로 인해 Slot 이 가지고있는 Widget들
			slot_hasitem->SlotHasItem_Visible(val);
		}
	}
	if (val) 
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCUserWidget_Inventory::Slot_Use(UCItem_Widget* Useslot)
{
	FSlot_Info info = Useslot->Get_Info();
	
	int32 idx = int32(info.SlotType);
	CheckTrue(5 <= idx); // Etc 아이템이라면 Return
	
	for(UCUserWidget_Slot* slot : Equip_Slots[int32(info.SlotType)])
	{
		if (slot->Possible_Check(Useslot)) // Slot 에 들어갈수있는지 체크
		{
			slot->Insert_Item(Useslot);
			slot->Equip_Item(Useslot);
			break;
		}
	}
}

void UCUserWidget_Inventory::EquipSlot_Use(UCUserWidget_Slot* Equipslot)
{ 
	/*FSlot_Info info = *Equipslot->Get_info();
	TPair<int32, int32> slotsize = Slot_Size(info);
	Slot_Swap(Equip_Slots[int32(info.SlotType)], Find_Slot(Inven_Slots, slotsize));*/

}

void UCUserWidget_Inventory::Init_InvenSlot()
{
	
	int Row_Cnt = -1;
	for (int i = 0; i < Inven_Slots.Num(); i++)
	{
		if (i % Column == 0)
			Row_Cnt++;
		Inven_Slots[i]->bTreasureSlot = false;
		inventory[Row_Cnt][i % Column] = Inven_Slots[i];
		inventory[Row_Cnt][i % Column]->Set_OwnerInventory(this);
		inventory[Row_Cnt][i % Column]->Slot_Position(FVector2D(Slot_X + Slot_RangeX * (i % Column), Slot_Y + Slot_RangeY * Row_Cnt)); // Slot Pos
	}
	Slot_xLink();
	Slot_yLink();
}
void UCUserWidget_Inventory::Init_TreasureSlot()
{

	int Row_Cnt = -1;
	for (int i = 0; i < Treasure_Slots.Num(); i++)
	{
		if (i % Treasure_Column == 0)
			Row_Cnt++;
		Treasure_inven[Row_Cnt][i % Treasure_Column] = Treasure_Slots[i];
		Treasure_inven[Row_Cnt][i % Treasure_Column]->Set_OwnerInventory(this);
		Treasure_inven[Row_Cnt][i % Treasure_Column]->Slot_Position(FVector2D(TreasureSlot_X + Slot_RangeX * (i % Treasure_Column), TreasureSlot_Y + Slot_RangeY * Row_Cnt)); // Slot Pos
	}
	Slot_xLink(Treasure_Row, Treasure_Column);
	Slot_yLink(Treasure_Row, Treasure_Column);
}

void UCUserWidget_Inventory::Slot_xLink()
{
	for (int i = 0; i < Row; i++)
	{
		for (int j = 0; j < Column; j++)
		{
			if (0 <= j - 1)	//	Left init
				inventory[i][j]->Dlink_Slot.Left_Slot = inventory[i][j - 1];
			if (j + 1 < Column) // Right init
				inventory[i][j]->Dlink_Slot.Right_Slot = inventory[i][j + 1];
		}
	}
}
void UCUserWidget_Inventory::Slot_yLink()
{
	for (int i = 0; i < Column; i++)
	{
		for (int j = 0; j < Row; j++)
		{
			if (0 <= j - 1)	//	Up_Slot init
				inventory[j][i]->Dlink_Slot.Up_Slot = inventory[j - 1][i];
			if (j + 1 < Row) // Down_Slot init
				inventory[j][i]->Dlink_Slot.Down_Slot = inventory[j + 1][i];
		}
	}
}


void UCUserWidget_Inventory::Slot_xLink(int Slots_Row, int Slots_Column)
{	//Treasure Custom
	for(int i =0; i < Slots_Row; i++)
	{
		for(int j =0; j < Slots_Column; j++)
		{
			if(0 <= j - 1)	//	Left init
				Treasure_inven[i][j]->Dlink_Slot.Left_Slot = Treasure_inven[i][j - 1];
			if(j + 1 < Slots_Column) // Right init
				Treasure_inven[i][j]->Dlink_Slot.Right_Slot = Treasure_inven[i][j + 1];
		}
	}
}
void UCUserWidget_Inventory::Slot_yLink(int Slots_Row, int Slots_Column)
{	//Treasure Custom
	for (int i = 0; i < Slots_Column; i++)
	{
		for (int j = 0; j < Slots_Row; j++)
		{
			if (0 <= j - 1)	//	Up_Slot init
				Treasure_inven[j][i]->Dlink_Slot.Up_Slot = Treasure_inven[j - 1][i];
			if (j + 1 < Slots_Row) // Down_Slot init
				Treasure_inven[j][i]->Dlink_Slot.Down_Slot = Treasure_inven[j + 1][i];
		}
	}
}

void UCUserWidget_Inventory::Slot_xLink(TArray<UCUserWidget_Slot*> slot_array, int Width, int pos_Index)
{
	int Row_Cnt = 0;
	for(int i = 0 ; i < slot_array.Num(); i++)
	{
		if (i % Width == 0 ) 
		{   // 각 행의 첫번째 idx 는 Right_Slot 만 연결
			Row_Cnt++;
			if(i + 1 < slot_array.Num())
				slot_array[i]->Dlink_Slot.Right_Slot = slot_array[i + 1];
		}
		else if (i  == (Width * Row_Cnt - 1) && i < slot_array.Num())
		{	// 각 행의 마지막 째 idx 는 Left_Slot 만 연결
			
			slot_array[i]->Dlink_Slot.Left_Slot = slot_array[i - 1];
		}
		else
		{	// 제외한 idx 는 양쪽 연결
			if (i + 1 < slot_array.Num())
				slot_array[i]->Dlink_Slot.Right_Slot = slot_array[i + 1];
			if ( 0 < i - 1)
				slot_array[i]->Dlink_Slot.Left_Slot  = slot_array[i - 1];
		}
		slot_array[i]->Slot_Position(FVector2D(Equip_PanelPos.X + EquipSlot_Pos[pos_Index].X + Slot_RangeX * (i % Width), Equip_PanelPos.Y + EquipSlot_Pos[pos_Index].Y + Slot_RangeY * Row_Cnt)); // Slot Pos
	

	}
	Slot_yLink(slot_array, Width);
}
void UCUserWidget_Inventory::Slot_yLink(TArray<UCUserWidget_Slot*> slot_array, int Width)
{
	for(int i =0; i < slot_array.Num();i++)
	{
		if(i + Width < slot_array.Num())
		{	// 위 행에 Slot이 있다면
			slot_array[i]->Dlink_Slot.Up_Slot = slot_array[i + Width];
			
		}
		if( 0 <= i - Width )
		{	// 아랫 행에 Slot이 있다면
			slot_array[i]->Dlink_Slot.Down_Slot = slot_array[i - Width];
			
		}
	}
}


void UCUserWidget_Inventory::Init_EquipSlot(TArray<UCUserWidget_Slot*> Head, TArray<UCUserWidget_Slot*> Sheath, TArray<UCUserWidget_Slot*> Upper, TArray<UCUserWidget_Slot*> body, TArray<UCUserWidget_Slot*> quick)
{
	Equip_Slots[0] = Head;
	Slot_xLink(Head, 2,0);
	
	Equip_Slots[1] = Sheath;
	Slot_xLink(Sheath, 1,1);
	
	Equip_Slots[2] = Upper;
	Slot_xLink(Upper, 2,2);

	Equip_Slots[3] = body;
	Slot_xLink(body, 2,3);
	
	Equip_Slots[4] = quick;
	Slot_xLink(quick, 1,4);

	
	for(TArray<UCUserWidget_Slot*> slots: Equip_Slots)
		for (UCUserWidget_Slot* slot : slots) 
		{
			slot->bTreasureSlot = false;
			slot->Set_OwnerInventory(this);
		}

}





void UCUserWidget_Inventory::Slot_Swap(UCUserWidget_Slot* slot1, UCUserWidget_Slot* slot2)
{
	CLog::Print("dsafasfasg");
	FSlot_Info temp_info = *slot1->Get_info();

	slot1->Set_info(slot2->Get_info());
	slot2->Set_info(&temp_info);
}

void UCUserWidget_Inventory::EquipSlot_Swap(UCUserWidget_Slot* slot1, UCUserWidget_Slot* slot2)
{
	// Case : slot1 is EquipSlot
	//CheckFalse(Equip_Slots[int32(slot2->Get_info()->SlotType)] == slot1); // SlotType Equal Check 
	//
	//FSlot_Info temp_info = *slot1->Get_info();

	//slot1->Set_info(slot2->Get_info());
	//slot2->Set_info(&temp_info);
}
