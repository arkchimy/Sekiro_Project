#include "Strategy_Pattern/Patrol_Strategy.h"
#include "Global.h"



FVector UPatrol_Strategy::Move(FVector Self_Loc)
{
	CLog::Log("Patrol_Action");//operation변경은 Notify로
	return FVector(0, 0, 0);
}

void UPatrol_Strategy::Action(FVector Target_Location, class UDataTable* Montage_Table, class AGameActor* OwnerActor, EMouseBut direction)
{
	CLog::Log("Patrol_Action");//operation변경은 Notify로
	
	// Action (FVector , Target = nullptr); 을 받고 
	// FVector 의 위치와 방향함수를 구함. Move를 통해 Interp를 통한 보간 회전후 방향이 Forward가 될시 Forward로 Montage실행
	// if R or L 일 경우 Move호출 해서 보간회전하기  
	CLog::Print(int32(direction));
	TArray<FName> Data_size = Montage_Table->GetRowNames();
	for (int i = 0; i < Data_size.Num(); i++)
	{
		FWeaponData_Enemy temp = *(Montage_Table->FindRow<FWeaponData_Enemy>(Data_size[i], Data_size[i].ToString()));
		if (temp.Input_But == direction && temp.Combat == ECombat_Monster::Partrol)
		{
			OwnerActor->PlayAnimMontage(temp.Data.Montage, temp.Data.PlayRatio, temp.Data.StartSection);
			return;
		}
	}

}

