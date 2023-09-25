#include "Actors/CThrowActors/CThrow_ManagerActor.h"
#include "Actors/CPlayer.h"
#include "Global.h"
#include "Actors/CThrowActors/CThrowItem.h"
#include "Actors/CThrowActors/CThrowItem_Arrow.h"


ACThrow_ManagerActor::ACThrow_ManagerActor()
{
	//PrimaryActorTick.bCanEverTick = true;
	
}



void ACThrow_ManagerActor::BeginPlay()
{
	int row = 0;
	for (TSubclassOf<ACThrowItem> data : Throwitem_class)
	{
		if (!!data)
		{
			for (int i = 0; i < 15; i++) //오브젝트 풀링 15 개씩 생성
			{

				ACThrowItem* item = GetWorld()->SpawnActor<ACThrowItem>(data);
				item->Set_Id(row);
				item->Set_Manager(this);
				Throw_Array.Add(item);

			}
		}
		row++;
	}
	Super::BeginPlay();

}


ACThrowItem* ACThrow_ManagerActor::Find_ThrowActor(int class_id)
{

	result_item = nullptr;
	
	for (ACThrowItem* data : Throw_Array)
	{
		if (!!data) 
		{
			if (data->Get_Id() != class_id)
				continue;
			if (!data->Get_Visible() && !data->Get_Throwing()) // 현재 나와있는 얘들은 제외.
			{
				result_item = data;
				data->Set_Throwing(true);
				
				return result_item;
			}
		}
	}
	CLog::Print("Thow_Item_Not_Found");
	return result_item;
}




