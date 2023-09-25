#include "CUserWidget_Slot.h"
#include "global.h"

#include "Engine/Texture2D.h"
#include "GameFramework/Character.h"

#include "CUserWidget_Inventory.h"
#include "CWidget_EquipSlot.h"
#include "UI_Inven/CItem_Widget.h"


void UCUserWidget_Slot::DragOver_Slot(UCItem_Widget* Insert_Item)
{
	// Drag 된 Mouse가 Over됬을떄
	
	bool result = Possible_Check(Insert_Item);
	
}
void UCUserWidget_Slot::DragLeave_Slot(UCItem_Widget* Insert_Item)
{
	//Drag 한 Mouse 가  Slot을 벗어났을때
	//Drag 놓을떄 호출 안 됌 확인완료
	FSlot_Info Insert_Item_info = Insert_Item->Get_Info();

	ESlotType type = Insert_Item_info.SlotType;
	Inventory->Reset_AllSlotTint();
}

void UCUserWidget_Slot::SlotHasItem_Visible(bool val)
{
	if (!!Item_Widget) 
	{
		Item_Widget->View_Item(val);
		
	}
}

bool UCUserWidget_Slot::Possible_Check(UCItem_Widget* Insert_Item)
{	
	// 위 결과에 따라 드래그 이동이 안됌
	FSlot_Info Insert_Item_info = Insert_Item->Get_Info();
	bool b = true;
	ESlotType type = Insert_Item_info.SlotType;
	TArray<UCUserWidget_Slot*> Check_Slot;
	b = IsValid_Slot(this, Insert_Item);
	Check_Slot.Add(this);
	switch(type)
	{
		case ESlotType::Upper_Weapon:
		case ESlotType::Body:
			if(b)
				b = IsValid_Slot(Dlink_Slot.Up_Slot, Insert_Item); // 위쪽 슬롯 검사
			if(b)
				b = IsValid_Slot(Dlink_Slot.Down_Slot, Insert_Item);// 아랫쪽 슬롯 검사.
			if(b)
				b = IsValid_Slot(Dlink_Slot.Right_Slot, Insert_Item); // 위쪽 슬롯 검사
			if (b)
			{
				// RigtSlot Nullptr avoid
				b = IsValid_Slot(Dlink_Slot.Right_Slot->Dlink_Slot.Up_Slot, Insert_Item);// 아랫쪽 슬롯 검사.
				if (b) // false : false 방지
					b = IsValid_Slot(Dlink_Slot.Right_Slot->Dlink_Slot.Down_Slot, Insert_Item); // 위쪽 슬롯 검사
			}

			if (!!Dlink_Slot.Up_Slot)
				Check_Slot.Add(Dlink_Slot.Up_Slot);
			if (!!Dlink_Slot.Down_Slot)
				Check_Slot.Add(Dlink_Slot.Down_Slot);
			if (!!Dlink_Slot.Right_Slot) 
			{
				Check_Slot.Add(Dlink_Slot.Right_Slot);
				if (!!Dlink_Slot.Right_Slot->Dlink_Slot.Up_Slot)
					Check_Slot.Add(Dlink_Slot.Right_Slot->Dlink_Slot.Up_Slot);
				if (!!Dlink_Slot.Right_Slot->Dlink_Slot.Down_Slot)
					Check_Slot.Add(Dlink_Slot.Right_Slot->Dlink_Slot.Down_Slot);
				
			}
			break;
		case ESlotType::Sheath_Weapon: 
		{

			b &= IsValid_Slot(Dlink_Slot.Up_Slot, Insert_Item); // 위쪽 슬롯 검사
			b &= IsValid_Slot(Dlink_Slot.Down_Slot, Insert_Item);// 아랫쪽 슬롯 검사.


			if (!!Dlink_Slot.Up_Slot)
				Check_Slot.Add(Dlink_Slot.Up_Slot);
			if (!!Dlink_Slot.Down_Slot)
				Check_Slot.Add(Dlink_Slot.Down_Slot);
		}
		break;
		case ESlotType::Etc1_1:
			break;
		case ESlotType::Etc2_2:
		case ESlotType::Head:
			if (b)
				b &= IsValid_Slot(Dlink_Slot.Up_Slot, Insert_Item);
			if (b)
				b &= IsValid_Slot(Dlink_Slot.Right_Slot, Insert_Item);
			if (b)
				b &= IsValid_Slot(Dlink_Slot.Right_Slot->Dlink_Slot.Up_Slot, Insert_Item);
			if (!!Dlink_Slot.Up_Slot)
				Check_Slot.Add(Dlink_Slot.Up_Slot);
			if (!!Dlink_Slot.Right_Slot)
			{
				Check_Slot.Add(Dlink_Slot.Right_Slot);
				if (!!Dlink_Slot.Right_Slot->Dlink_Slot.Up_Slot)
					Check_Slot.Add(Dlink_Slot.Right_Slot->Dlink_Slot.Up_Slot);
			}
			break;
		case ESlotType::Etc1_2:
			if (b)
				b &= IsValid_Slot(Dlink_Slot.Up_Slot, Insert_Item);
			if (!!Dlink_Slot.Up_Slot)
				Check_Slot.Add(Dlink_Slot.Up_Slot);
			break;
		case ESlotType::Etc2_1:
			if (b)
				b &= IsValid_Slot(Dlink_Slot.Right_Slot, Insert_Item);
			if (!!Dlink_Slot.Right_Slot)
			{
				Check_Slot.Add(Dlink_Slot.Right_Slot);
			}
			break;
		
	}
	if (b) 
	{
		for(UCUserWidget_Slot* slot : Check_Slot)
		{
			slot->Possible_Tint();
		}
		
		
		return true;
	}
	else
	{
		for (UCUserWidget_Slot* slot : Check_Slot)
		{
			slot->Impossible_Tint();
		}

		
	}
	return false;
}

bool UCUserWidget_Slot::IsValid_Slot(UCUserWidget_Slot* isvalid, UCItem_Widget* Same_Widget)
{
	if (!!isvalid) 
	{	// isvalid is Not Null
		if (!!isvalid->Get_ItemWidget())
		{	// has item
			if (isvalid->Get_ItemWidget() == Same_Widget)
			{
				return true;
			}
			return false;
		}
		if (isvalid->IsUsed_Slot()) 
		{
			return false;
		}
		return true;
	}
	return false;
}

void UCUserWidget_Slot::Set_info(FSlot_Info* data)
{
	// Item_Widget 을 가지고 있을때
	info = *data;
	Used_Slot = true;
	//img = info.Img;
	//Slot_Update(img);
}
void UCUserWidget_Slot::Slot_Position(FVector2D pos)
{
	Slot_pos = pos;
}
void UCUserWidget_Slot::Slot_Use()
{
	CheckTrue(Get_info()->SlotType == ESlotType::Max); // 비어있지않다면
	
	//Inventory->Slot_Use(this);
}

void UCUserWidget_Slot::Insert_Item(UCItem_Widget* input_item)
{
	//크기 체크 아직 안함
	if (Possible_Check(input_item))
	{	//크기 체크 후
		FSlot_Info tempinfo = input_item->Get_Info();
		if(tempinfo.SlotType == ESlotType::Sheath_Weapon)
			input_item->Insert_Slot(Slot_pos,this);
		else if(tempinfo.SlotType == ESlotType::Body || tempinfo.SlotType == ESlotType::Upper_Weapon)
			input_item->Insert_Slot(FVector2D(Slot_pos.X + 45,Slot_pos.Y), this);
		else if(tempinfo.SlotType == ESlotType::Etc2_1 || tempinfo.SlotType == ESlotType::Etc2_2)
			input_item->Insert_Slot(FVector2D(Slot_pos.X + 45, Slot_pos.Y - 45), this);
		else if (tempinfo.SlotType == ESlotType::Etc1_1)
			input_item->Insert_Slot(FVector2D(Slot_pos.X + 5, Slot_pos.Y + 5), this);
		else if (tempinfo.SlotType == ESlotType::Etc1_2)
			input_item->Insert_Slot(FVector2D(Slot_pos.X + 5, Slot_pos.Y - 25), this);
		else
			input_item->Insert_Slot(Slot_pos, this);
		
	}
	Inventory->Reset_AllSlotTint();
	

}




void UCUserWidget_Slot::Slot_Swap( UCUserWidget_Slot* swap_slot)
{
	CheckNull(swap_slot);

	CheckTrue(swap_slot->Get_info()->SlotType == ESlotType::Max);
	Inventory->Slot_Swap(this, swap_slot);
}

void UCUserWidget_Slot::Set_LinkItemWidget(UCItem_Widget* input_Item)
{
	FSlot_Info Insert_Item_info = input_Item->Get_Info();
	ESlotType type = Insert_Item_info.SlotType;
	TArray<UCUserWidget_Slot*> Check_Slot;
	Set_ItemWidget(input_Item);
	switch (type)
	{
		case ESlotType::Upper_Weapon:
		case ESlotType::Body:
			// (2,3)
			Dlink_Slot.Up_Slot->Set_ItemWidget(input_Item);
			Dlink_Slot.Down_Slot->Set_ItemWidget(input_Item);
			Dlink_Slot.Right_Slot->Set_ItemWidget(input_Item);
			Dlink_Slot.Right_Slot->Dlink_Slot.Up_Slot->Set_ItemWidget(input_Item);
			Dlink_Slot.Right_Slot->Dlink_Slot.Down_Slot->Set_ItemWidget(input_Item);
		
			break;
		case ESlotType::Sheath_Weapon:
		{	// (1,3)
			Dlink_Slot.Up_Slot->Set_ItemWidget(input_Item);
			Dlink_Slot.Down_Slot->Set_ItemWidget(input_Item);
		
		}
		break;
		case ESlotType::Etc1_1:
			break;
		case ESlotType::Etc2_2:
		case ESlotType::Head:
			Dlink_Slot.Up_Slot->Set_ItemWidget(input_Item);
			Dlink_Slot.Right_Slot->Set_ItemWidget(input_Item);
			Dlink_Slot.Right_Slot->Dlink_Slot.Up_Slot->Set_ItemWidget(input_Item);
			break;
		case ESlotType::Etc1_2:
			Dlink_Slot.Up_Slot->Set_ItemWidget(input_Item);
			break;
		case ESlotType::Etc2_1:
			Dlink_Slot.Right_Slot->Set_ItemWidget(input_Item);
			break;

	}
	

}

void UCUserWidget_Slot::Reset_LinkItemWidget(ESlotType type)
{
	Set_ItemWidget(nullptr);
	switch (type)
	{
		case ESlotType::Upper_Weapon:
		case ESlotType::Body:
		{
			Dlink_Slot.Up_Slot->Set_ItemWidget(nullptr);
			Dlink_Slot.Down_Slot->Set_ItemWidget(nullptr);
			Dlink_Slot.Right_Slot->Set_ItemWidget(nullptr);
			Dlink_Slot.Right_Slot->Dlink_Slot.Up_Slot->Set_ItemWidget(nullptr);
			Dlink_Slot.Right_Slot->Dlink_Slot.Down_Slot->Set_ItemWidget(nullptr);
		}
		break;
		case ESlotType::Sheath_Weapon:
		{
			Dlink_Slot.Up_Slot->Set_ItemWidget(nullptr);
			Dlink_Slot.Down_Slot->Set_ItemWidget(nullptr);

		}
		case ESlotType::Etc1_1:
			break;
		case ESlotType::Etc2_2:
		case ESlotType::Head:
			Dlink_Slot.Up_Slot->Set_ItemWidget(nullptr);
			Dlink_Slot.Right_Slot->Set_ItemWidget(nullptr);
			Dlink_Slot.Right_Slot->Dlink_Slot.Up_Slot->Set_ItemWidget(nullptr);
			break;
		case ESlotType::Etc1_2:
			Dlink_Slot.Up_Slot->Set_ItemWidget(nullptr);
			break;
		case ESlotType::Etc2_1:
			Dlink_Slot.Right_Slot->Set_ItemWidget(nullptr);
			break;

	}
}

bool UCUserWidget_Slot::IsUsed_Slot()
{
	if (!!Item_Widget)
		return true;
	return false;
	
}
void UCUserWidget_Slot::Setting_EquipSlot(ESlotType input)
{
	info.SlotType = input;
}

void UCUserWidget_Slot::Set_OwnerInventory(class UCUserWidget_Inventory* Owner_Inven)
{
	Inventory = Owner_Inven;
}
