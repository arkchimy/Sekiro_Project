#include "Strategy_Pattern/Patrol_Strategy.h"
#include "Global.h"



FVector UPatrol_Strategy::Move(FVector Self_Loc)
{
	CLog::Log("Patrol_Action");//operation변경은 Notify로
	return FVector(0, 0, 0);
}

void UPatrol_Strategy::Action(class UDataTable* Montage_Table, class AGameActor* OwnerActor, EMouseBut direction)
{
	CLog::Log("Patrol_Action");//operation변경은 Notify로
	
	int ran = rand() % 5;
	switch (ran)
	{
	case 0 : 
		direction = EMouseBut::Right;
		break;
	case 1 : 
		direction = EMouseBut::Left;
		break;
	case 2:
		direction = EMouseBut::Back;
		break;
	default:
		direction = EMouseBut::Front;
		
	}
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

