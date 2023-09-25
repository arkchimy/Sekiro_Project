#include "Actors/CAttackments/CAttackment_ArcherEnemy.h"
#include "Global.h"

#include "ActorComponents/State_Component/CStateComponent.h"
#include "Actors/CEnemy.h"

#include "Actors/CThrowActors/CThrowItem.h"
#include "Actors/CThrowActors/CThrow_AttachDummy.h"
#include "Actors/CThrowActors/CThrow_ManagerActor.h"




void ACAttackment_ArcherEnemy::Do_LAttack()
{
	//if (State->IsIdleState())
	//{
	//	State->Set_ActionMode();
	//	Attack_Cnt = UKismetMathLibrary::RandomInteger(L_PushDatas.Num());
	//	OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(L_PushDatas[Attack_Cnt].Montage, L_PushDatas[Attack_Cnt].PlayRatio);
	//	LastDatas = L_PushDatas[Attack_Cnt]; // Montage따른 공격력 적용
	//	if (!L_PushDatas[Attack_Cnt].bCanMove)
	//	{
	//		State->Set_UnMove();

	//	}
	//	if (AttackTag_Delegate.IsBound())
	//	{
	//		AttackTag_Delegate.Broadcast(L_PushDatas[Attack_Cnt].Tags);
	//	}
	//	
	//}
	
}

void ACAttackment_ArcherEnemy::Begin_Attack()
{

}

void ACAttackment_ArcherEnemy::End_Attack()
{
	State->Set_IdleMode();
}
void ACAttackment_ArcherEnemy::L_AttackRelease()
{
	/*FVector start, end, direction;
	enemy->GetLocationAndDirection(start, end, direction);

	if (PressVal > 2.0f)
	{
		if (!!causer)
		{
			causer->SetActorRotation(direction.Rotation());
			causer->Set_Pressed(PressVal * 3, OwnerCharacter);
		

		}
		else 
		{
			CLog::Print("causer is Null");
		}
	}*/
	

}

void ACAttackment_ArcherEnemy::Reload_Arraw()
{
	
	/*if (!!enemy) 
		causer = enemy->Get_ThrowManager()->Find_ThrowActor(0);
	if (!!causer)
	{
		if (causer->ThrowComponentHit.IsBound())
			causer->ThrowComponentHit.RemoveDynamic(this, &ACAttackment_ArcherEnemy::OnBeginOverlap);
		causer->ThrowComponentHit.AddDynamic(this, &ACAttackment_ArcherEnemy::OnBeginOverlap);
	}
	if (!!enemy && !!Dummy)
		Dummy = enemy->Get_ThrowDummy();
	if (!!Dummy && !!causer)
	{
		Dummy->Set_StaticMesh(causer->Get_ThrowMesh());
		Dummy->SetActorRelativeRotation(FQuat(causer->Edit.Rot));
		Dummy->SetActorScale3D(causer->Edit.Scale);
	}*/
}

void ACAttackment_ArcherEnemy::OnBeginOverlap(const FHitResult & Hit, float power, AActor * Causer)
{
	
	/*FDamageEvent Event_Type;
	AController* controll = enemy->GetController();
	
	if (!!enemy)
	{
		controll = enemy->GetController();
	}

	Hit.Actor->TakeDamage(power, Event_Type, controll, Causer);*/
}
