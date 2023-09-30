#include "Actors/CAttackments/CEnemy_Melee.h"
#include "Actors/GameActor.h"

#include "Global.h"

#include "GameFramework/Character.h"

#include "ActorComponents/State_Component/CStateComponent.h"
#include "ActorComponents/CStatusComponent.h"

#include "Sound/SoundCue.h"
void ACEnemy_Melee::BeginPlay()
{
	PrimaryActorTick.bCanEverTick = true;
	Super::BeginPlay();
	AttachToActor(OwnerCharacter, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
}

void ACEnemy_Melee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACEnemy_Melee::Monster_Attack()
{
	//if (AttackTag_Delegate.IsBound()) //  Enemy 에서만 실행 됨
	//	AttackTag_Delegate.Broadcast(LastDatas.Collision_Tag);
}

UAnimMontage* ACEnemy_Melee::Get_NextMontage()
{
	//if (rand() % 2 == 0)
	//	Button_Event(EMouseBut::Left);
	//else
	//	Button_Event(EMouseBut::Right);
	//if (AttackTag_Delegate.IsBound()) //  Enemy 에서만 실행 됨
	//	AttackTag_Delegate.Broadcast(LastDatas.Collision_Tag);

	//return LastDatas.Data.Montage;
	return nullptr;
}

//void ACEnemy_Melee::Do_LAttack()
//{
//	Button_Event(EMouseBut::Left);
//
//	if(AttackTag_Delegate.IsBound()) //  Enemy 에서만 실행 됨
//		AttackTag_Delegate.Broadcast(LastDatas.Collision_Tag);
//}
//
//void ACEnemy_Melee::Do_RAttack()
//{
//	Button_Event(EMouseBut::Right);
//	if (AttackTag_Delegate.IsBound()) //  Enemy 에서만 실행 됨
//		AttackTag_Delegate.Broadcast(LastDatas.Collision_Tag);
//}


void ACEnemy_Melee::Begin_Attack()
{
}

void ACEnemy_Melee::End_Attack()
{
	State->Set_IdleMode();
	Nextable = false;

	//if(rand()%3 == 0) // 가끔 초기화
	//	Attack_Cnt = 0;
}

void ACEnemy_Melee::Button_Event(EMouseBut input)
{
	//if (State->IsIdleState()) // 공격의 시작
	//{
	//	Seach_ComboIndex(input); //Attack_Cnt Init 
	//	State->Set_ActionMode();

	//	LastDatas = ComboData;// Hit Stop 을 적용하려고 index 저장 개념
	//	Montage_Play(ComboData);
	//}
}

void ACEnemy_Melee::Montage_Play(FWeaponData_Enemy input)
{
	//OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(input.Data.Montage, input.Data.PlayRatio);
	
	//OwnerCharacter = Cast<ACharacter>(GetOwner());
	//OwnerCharacter->PlayAnimMontage(input.Data.Montage, input.Data.PlayRatio);
	
}

void ACEnemy_Melee::Montage_Play(FWeaponData_Enemy input, FName Section)
{
	//OwnerCharacter = Cast<ACharacter>(GetOwner());
	//OwnerCharacter->PlayAnimMontage(input.Data.Montage, input.Data.PlayRatio, Section);

}

void ACEnemy_Melee::Player_AttackSound()
{

}

void ACEnemy_Melee::TakeOverlap_Actor(const FHitResult& Hit, FVector Hit_location, AActor* Causer, EAttackTag_Type AttackTag, float KnockBack_Power)
{
	FDamageEvent Damage_Type;
	AController* controll = OwnerCharacter->GetController();
	
	AGameActor* target = Cast<AGameActor>(Hit.Actor);
	/*if (!!target)
		target->Set_Attack_Tag(LastDatas.Attack_Tag, LastDatas.HittedEffect);*/
	
	AGameActor* HittedActor = Cast<AGameActor>(Hit.Actor);
	if (!!HittedActor) {
		HittedActor->TakeOverlap_SetAttackTag(AttackTag, KnockBack_Power); // AttackTag,넉백 정보 set
		HittedActor->Set_HittedEffect(LastDatas.Data.HittedEffect);
	}
	//, LastDatas.HittedEffect
	Hit.Actor->TakeDamage(LastDatas.Power, Damage_Type, controll, Causer);// 

	FTransform transform = Effect_Trans;

	ACharacter* overlap_Actor = Cast<ACharacter>(Hit.GetActor());
	FName hit_bone = Hit.BoneName;

	if (!!overlap_Actor)
	{
		FTransform trans;
		trans.SetLocation(Hit_location);
		trans.SetRotation(FQuat(GetActorRotation()));
	}

}

void ACEnemy_Melee::Seach_ComboIndex(EMouseBut but)
{
	while (Attack_Cnt < ComboRow.Num())
	{
		FWeaponData_Enemy temp = *(ComboTable->FindRow<FWeaponData_Enemy>(ComboRow[Attack_Cnt], ComboRow[Attack_Cnt].ToString()));
		if (temp.Input_But == but)
		{
			ComboData = temp;
			Attack_Cnt++;
			return;
		}
		else
			Attack_Cnt++;
	}
	Attack_Cnt = 0; // 못찾을 경우
	while (Attack_Cnt < ComboRow.Num())
	{
		FWeaponData_Enemy temp = *(ComboTable->FindRow<FWeaponData_Enemy>(ComboRow[Attack_Cnt], ComboRow[Attack_Cnt].ToString()));
		if (temp.Input_But == but)
		{
			ComboData = temp;
			Attack_Cnt++;
			return;
		}
		else
			Attack_Cnt++;
	}
}
