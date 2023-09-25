#include "CGame_HUD.h"
#include "Global.h"

#include "CUserWidget_Inventory.h"
#include "GameFramework/PlayerController.h"
#include "Actors/GameActor.h"

#include "Actors/TreasureBox/CTreasureBox_Master.h"


void UCGame_HUD::Open_Inventory()
{
	CheckNull(Inventory);
	Inventory->Open_Inventory();
	// 이제 Blue Print 함수들 전부 C++ 로 옮기기 
}
void UCGame_HUD::Open_TreasureBox(TArray<FInven_Item> Treasureitems, class ACTreasureBox_Master* box)
{
	CheckNull(Inventory);
	
	Inventory->TreasureBox_Update(Treasureitems);
	Inventory->Set_TreasureBox(box);
	Inventory->Open_TreasureBox();
	
	// 이제 Blue Print 함수들 전부 C++ 로 옮기기 
}
void UCGame_HUD::Close_TreasureBox()
{
	CheckNull(Inventory);
	Inventory->Set_TreasureBox(nullptr);
	Inventory->Close_TreasureBox();

	// 이제 Blue Print 함수들 전부 C++ 로 옮기기 
}

void UCGame_HUD::View_EnemyUI(class AGameActor* target)
{
	EnemyUI_Init(target);
	// 이제 Blue Print 함수들 전부 C++ 로 옮기기 
}

bool UCGame_HUD::Item_PickUp(int id)
{
	return Inventory->Item_PickUp(id);
}


void UCGame_HUD::Set_Inventory(UCUserWidget_Inventory* inven, class APlayerController* Controller)
{
	Inventory = inven; 
	Inventory->Init_MouseControl(Controller);
	

}

