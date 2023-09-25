#include "Actors/CAttackments/CAttackment_Enemy.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"

#include "GameFramework/Character.h"

#include "AIController.h"
#include "Actors/Enemies/CAIController_Enemy.h"

#include "ActorComponents/State_Component/CStateComponent.h"
#include "ActorComponents/CStatusComponent.h"


void ACAttackment_Enemy::BeginPlay()
{
	//PrimaryActorTick.bCanEverTick = true;
	Super::BeginPlay();

}


void ACAttackment_Enemy::Do_LAttack()
{
	//if (State->IsIdleState())
	//{
	//	State->Set_ActionMode();
	//	Attack_Cnt = UKismetMathLibrary::RandomInteger(L_PushDatas.Num());
	//	Attack_Cnt = 0;//임시
	//	OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(L_PushDatas[Attack_Cnt].Montage, L_PushDatas[Attack_Cnt].PlayRatio);
	//	LastDatas = L_PushDatas[Attack_Cnt]; // Montage따른 공격력 적용
	//	if (!L_PushDatas[Attack_Cnt].bCanMove) 
	//	{
	//		State->Set_UnMove();

	//	}
	//	if(AttackTag_Delegate.IsBound())
	//	{
	//		AttackTag_Delegate.Broadcast(L_PushDatas[Attack_Cnt].Tags);
	//	}
	//}
}


void ACAttackment_Enemy::Begin_Attack()
{

}

void ACAttackment_Enemy::End_Attack()
{
	//CheckTrue(NextActtack); // 다음 공격으로 될경우 Idle 로 바꾸지않음.
	/*if(!!enemy)
		enemy->GetMesh()->GetAnimInstance()->Montage_Stop(0.01f);*/
	State->Set_IdleMode();
	
	
}

void ACAttackment_Enemy::Resiste_PlayMontage()
{
	
	//enemy->PlayAnimMontage(Special_AttackDatas[0].Resiste_Montage, Special_AttackDatas[0].Resiste_PlayRatio);
}

void ACAttackment_Enemy::TakeOverlap_Actor(const FHitResult& Hit, FVector Hit_location,AActor* Causer, EAttackTag_Type AttackTag, float KnockBack_Power)
{
	
	//FDamageEvent Damage_Type;
	//AController* controll = enemy->GetController();
	//
	//if (!!enemy)  
	//{
	//	controll = enemy->GetController();
	//	
	//}
	//if(enemy->Get_Interactive())
	//{
	//	
	//	enemy->PlayAnimMontage(Special_AttackDatas[0].Try_Montage, Special_AttackDatas[0].Try_PlayRatio);
	//	ACPlayer* playertemp = Cast<ACPlayer>(Hit.Actor);
	//	ACPlayer* weapontemp = Cast<ACPlayer>(Hit.Actor->GetOwner()); //무기가 맞았을경우.
	//	if (!!playertemp) 
	//	{
	//		playertemp->Stren_Damaged(enemy, Special_AttackDatas[0].Tag);

	//		ACAIController_Enemy* controller = Cast<ACAIController_Enemy>(enemy->Controller);
	//		if (!!controller)
	//		{
	//			
	//			controller->MoveUpdate(false);
	//		}
	//		else
	//			CLog::Print("Controller trans Susess");
	//	}
	//	else if (!!weapontemp) 
	//	{
	//		
	//		weapontemp->Stren_Damaged(enemy, Special_AttackDatas[0].Tag);

	//	}
	//}
	//else 
	//{
	//	Hit.Actor->TakeDamage(L_PushDatas[Attack_Cnt].Power, Damage_Type, controll, Causer);
	//	FTransform transform = L_PushDatas[Attack_Cnt].EffectTransform;

	//	ACharacter* overlap_Actor = Cast<ACharacter>(Hit.GetActor());
	//	FName hit_bone = Hit.BoneName;

	//	if (!!overlap_Actor)
	//	{
	//		FTransform trans;
	//		trans.SetLocation(Hit_location);
	//		trans.SetRotation(FQuat(GetActorRotation()));
	//		Effect_Trans = trans;

	//		//effect 형성
	//	}
	//}


}
