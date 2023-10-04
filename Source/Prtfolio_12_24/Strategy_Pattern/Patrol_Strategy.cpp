#include "Strategy_Pattern/Patrol_Strategy.h"
#include "Global.h"



FVector UPatrol_Strategy::Move(FVector Goal_Loc, class AGameActor* TargetActor)
{
	if (!!TargetActor)
		return TargetActor->GetActorLocation();
	return FVector(0,0,0);
}

void UPatrol_Strategy::Action(FVector Target_Location, class UDataTable* Montage_Table, class AGameActor* OwnerActor, EMouseBut direction)
{
	CLog::Log("Patrol_Action");//operation������ Notify��
	
	// Action (FVector , Target = nullptr); �� �ް� 
	// FVector �� ��ġ�� �����Լ��� ����. Move�� ���� Interp�� ���� ���� ȸ���� ������ Forward�� �ɽ� Forward�� Montage����
	// if R or L �� ��� Moveȣ�� �ؼ� ����ȸ���ϱ�  
	
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

