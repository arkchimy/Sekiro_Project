#include "UI_Inven/CItem_Widget.h"
#include "Global.h"
#include "UI_Inven/CUserWidget_Inventory.h"
#include "CUserWidget_Slot.h"
#include "Sound/SoundCue.h"


void UCItem_Widget::Set_Info(FSlot_Info input)
{
	info = input;
}
FSlot_Info UCItem_Widget::Get_Info()
{
	return info;
}

void UCItem_Widget::Insert_Slot(FVector2D pos, UCUserWidget_Slot* slot)
{
	if (!!Current_Slot) 
	{	//Current_Slot를 이미 가지고있었을 경우  있던자리 Link Free 
		Current_Slot->Reset_LinkItemWidget(info.SlotType);
		// Current_slot과 Swap_slot이 Treasure Slot 인지 Inven Slot인지 판별하기
		if(Current_Slot->bTreasureSlot != slot->bTreasureSlot)
		{
			if (Current_Slot->bTreasureSlot) 
			{
				if(!!Inventory)
				{
					Inventory->Treasure_Remove(info.id);
				}
				else
				{
					CLog::Print("OwnerInven is Null");
				}
				CLog::Print("Treasure = > Inven");
			}
			else 
			{
				if (!!Inventory)
				{
					Inventory->Treasure_Add(info.id);
				}
				else
				{
					CLog::Print("OwnerInven is Null");
				}
				CLog::Print("Inven = > Treasure");
			}
		}
		if (!!info.Drop_Sound) 
		{
			AActor* Owner_Actor = Inventory->Get_Owner();
			if(!!Owner_Actor)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), info.Drop_Sound, Inventory->Get_Owner()->GetActorLocation());
		}
	}
	FWidgetTransform trans = RenderTransform;
	trans.Translation = FVector2D(pos.X,pos.Y);
	SetRenderTransform(trans);

	Current_Slot = slot;
	Current_Slot->Set_LinkItemWidget(this);
	//Inventory->All_Slot_Hit(true);
}

void UCItem_Widget::Double_ClickEvent()
{
	CheckNull(Inventory);
	Current_Slot->Reset_LinkItemWidget(info.SlotType);
	Inventory->Slot_Use(this);
	Inventory->All_Slot_Hit(true);
	if(!!info.Use_Sound)
	{
		AActor* Owner_Actor = Inventory->Get_Owner();
		if (!!Owner_Actor)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), info.Use_Sound, Inventory->Get_Owner()->GetActorLocation());
	}

}

void UCItem_Widget::All_Slot_Hit(bool val)
{
	CheckNull(Inventory);
	Inventory->All_Slot_Hit(val);
}

void UCItem_Widget::Throw_on_thefloor()
{
	CheckNull(Current_Slot);
	CheckNull(Inventory);
	if (Current_Slot->bTreasureSlot)
	{	// Treasure  => Floor
		Inventory->Throw_TreasureItem(info.id);
		Current_Slot->Reset_LinkItemWidget(info.SlotType);
	}
	else
	{	// Inven => Floor
		Inventory->Throw_InvenItem(info.id);
		Current_Slot->Reset_LinkItemWidget(info.SlotType);
	}
	SetVisibility(ESlateVisibility::Hidden);
	//Inventory->Reset_ItemWidget(this);
}
