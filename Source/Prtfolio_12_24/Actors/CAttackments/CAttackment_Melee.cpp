#include "CAttackment_Melee.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Engine/DataTable.h"

#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"
#include "Actors/GameActor.h"

#include "ActorComponents/State_Component/CStateComponent.h"
#include "ActorComponents/CStatusComponent.h"
#include "Sound/SoundCue.h"

void ACAttackment_Melee::BeginPlay()
{
	PrimaryActorTick.bCanEverTick = true;
	Super::BeginPlay();

}
void ACAttackment_Melee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACAttackment_Melee::NextAttack_Play()
{
	
	CheckFalse(NextActtack);
	
	NextActtack = false;
	State->Set_ActionMode();
	Montage_Play(ComboData); // 다음 공격이 들어있음.

	LastDatas = ComboData;
	if (ComboData.LastAttack) // 마지막 공격이라면
	{
		Attack_Cnt = 0;
		ComboData = *(ComboTable->FindRow<FWeaponData_Player>(ComboRow[Attack_Cnt], ComboRow[Attack_Cnt].ToString()));
	}
	
	
	
}

void ACAttackment_Melee::Button_Event(EMouseBut input)
{
	
	if (input == EMouseBut::Left)
	{

		if (!R_push_Chk)  // R버튼이  push 중이 아닐떄. 
		{
			if (State->IsIdleState()) // 공격의 시작
			{
				Seach_ComboIndex(input); //Attack_Cnt Init 
				State->Set_ActionMode();

				LastDatas = ComboData;// Hit Stop 을 적용하려고 index 저장 개념
				Montage_Play(ComboData);

				if (!ComboData.bCanMove)
					State->Set_UnMove();

			}
			else if(State->IsRush() && Nextable) // Rush 공격
			{
				CheckFalse(Nextable);
				Seach_ComboIndex(EMouseBut::Shift_Attack); //Attack_Cnt Init 

				NextActtack = true; // 다음 공격 실행 예정 여부
				Nextable = false;	// 중복 클릭 방지

				if (!ComboData.bCanMove)
					State->Set_UnMove();
			}
			else if (State->IsActionState() && Nextable) // Comboable 단계에서 마우스를 눌렀다면
			{
				CheckFalse(Nextable);
				Seach_ComboIndex(input); //Attack_Cnt Init 

				NextActtack = true; // 다음 공격 실행 예정 여부
				Nextable = false;	// 중복 클릭 방지

				if (!ComboData.bCanMove)
					State->Set_UnMove();

			}

		}
		if (R_push_Chk && Nextable) // 마우스 양쪽 누름  
		{
			Seach_ComboIndex(EMouseBut::LR); //Attack_Cnt Init 

			R_push_Chk = false;
			NextActtack = true; // 다음 공격 실행 예정 여부
			Nextable = false;	// 중복 클릭 방지

			if (!ComboData.bCanMove)
				State->Set_UnMove();
		}
		CheckFalse(L_push_Chk);

	}
	else if (input == EMouseBut::Right)
	{
		if (State->IsIdleState()) // Guard의 시작
		{
			R_push_Chk = true;

			Seach_ComboIndex(input); //Attack_Cnt Init 
			State->Set_BlockMode();// Guard의 시작

			LastDatas = ComboData;
			Montage_Play(ComboData,FName("Loop"));
			
			if (!ComboData.bCanMove)
				State->Set_UnMove();

		}
		else if (State->IsActionState()) // Comboable 단계에서 마우스를 눌렀다면
		{
			CheckFalse(Nextable);
			CheckTrue(R_push_Chk);

			R_push_Chk = true;
			Seach_ComboIndex(input); //Attack_Cnt Init 
			NextActtack = true; // 다음 공격 실행 예정 여부
			Nextable = false;	// 중복 클릭 방지

			if (!ComboData.bCanMove)
				State->Set_UnMove();

		}
		else if (State->IsBlockState() && R_push_Chk) 
		{	// Block 성공시
			Seach_ComboIndex(input); //Attack_Cnt Init 
			LastDatas = ComboData;
			Montage_Play(ComboData);

			if (!ComboData.bCanMove)
				State->Set_UnMove();

		}



	}
	else
	{
		if (State->IsIdleState()) // Action의 시작
		{

			Seach_ComboIndex(input); //Attack_Cnt Init 
			State->Set_ActionMode();
			LastDatas = ComboData;
			Montage_Play(ComboData);

			if (!ComboData.bCanMove)
				State->Set_UnMove();

		}
		
	}
	
}

void ACAttackment_Melee::Montage_Play(FWeaponData_Player input)
{
	OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(input.Data.Montage, input.Data.PlayRatio);
}
void ACAttackment_Melee::Montage_Play(FWeaponData_Player input, FName Section)
{
	OwnerCharacter->PlayAnimMontage(input.Data.Montage, input.Data.PlayRatio,Section);
}

void ACAttackment_Melee::T_Attack()
{
	Button_Event(EMouseBut::T_Attack);
}


void ACAttackment_Melee::R_Attack()
{
	Button_Event(EMouseBut::R_Attack);
}

void ACAttackment_Melee::Do_LAttack()
{
	
	Button_Event(EMouseBut::Left);
}


void ACAttackment_Melee::Do_RAttack()
{
	Button_Event(EMouseBut::Right);
}

void ACAttackment_Melee::Do_Parrying()
{
	R_push_Chk = false;
	
	EKeybord dic = OwnerActor->Get_LastKey();
	if (dic == EKeybord::Left)
		Seach_ComboIndex(EMouseBut::L_LR); //Attack_Cnt Init 
	else if (dic == EKeybord::Right)
		Seach_ComboIndex(EMouseBut::R_LR);
	else 
		Seach_ComboIndex(EMouseBut::LR);
	LastDatas = ComboData;

	Montage_Play(LastDatas);

	if (!ComboData.bCanMove)
		State->Set_UnMove();
}

void ACAttackment_Melee::SuddenAttack()
{
	
	if (State->IsIdleState()) // 공격의 시작
	{
		State->Set_ActionMode();
		
		Seach_ComboIndex(EMouseBut::Back_Attack);
		LastDatas = ComboData;

		Montage_Play(LastDatas);

		if (!ComboData.bCanMove)
			State->Set_UnMove();
	}
}

void ACAttackment_Melee::L_AttackRelease()
{
	

}

void ACAttackment_Melee::R_AttackRelease()
{
	
	R_push_Chk = false;


}

void ACAttackment_Melee::Pressed_But()
{
	//CheckFalse(R_push_Chk);
	if (R_push_Chk) 
	{	// Gurad State
		//Montage_Play(LastDatas, FName("Loop"));
	}
	else
		Montage_Play(LastDatas, FName("LoopEnd"));
}

void ACAttackment_Melee::Begin_Attack()
{

}

void ACAttackment_Melee::End_Attack()
{
	State->Set_IdleMode();	
	Attack_Cnt = 0;
	Nextable = false;
}



void ACAttackment_Melee::Player_AttackSound()
{
	FVector location = OwnerCharacter->GetActorLocation();
	FRotator rotator = FRotator::ZeroRotator;
	FTransform trans;
	trans.SetLocation(location);
	trans.SetRotation(FQuat(rotator));
	trans.SetScale3D(FVector::OneVector);

	if (!!Attack_Sound) // Sound 체크만
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Attack_Sound, location);
}

void ACAttackment_Melee::TakeOverlap_Actor(const FHitResult& Hit, FVector Hit_location,AActor* Causer, EAttackTag_Type AttackTag, float KnockBack_Power)
{
	
	FDamageEvent Damage_Type;
	AController* controll = player->GetController();

	AGameActor* HittedActor = Cast<AGameActor>(Hit.Actor);
	if (!!HittedActor) 
	{
		HittedActor->TakeOverlap_SetAttackTag(AttackTag, KnockBack_Power);
		FRotator hit_rot = UKismetMathLibrary::FindLookAtRotation(HittedActor->GetActorLocation(), Hit_location);
		HittedActor->hit_roation = hit_rot;
		
		HittedActor->Set_HittedEffect(LastDatas.Data.HittedEffect);
	}

	if (!!player)
	{
		ACEnemy* target = Cast<ACEnemy>(Hit.Actor);
		if (!!target) 
		{
			target->Set_HitBone(Hit.BoneName);
			//target->Set_Attack_Tag(LastDatas.Attack_Tag);
		}
	}
	else if (!!enemy)  
	{
		controll = enemy->GetController();
		
	}
	
	Hit.Actor->TakeDamage(LastDatas.Power, Damage_Type, controll, Causer);//여기
	FTransform transform = Effect_Trans;

	ACharacter* overlap_Actor = Cast<ACharacter>(Hit.GetActor());
	FName hit_bone = Hit.BoneName;
	
	if (!!overlap_Actor) 
	{
		//FVector hit_location = overlap_Actor->GetMesh()->GetSocketLocation(hit_bone);
		FTransform trans;
		trans.SetLocation(Hit_location);
		trans.SetRotation(FQuat(GetActorRotation()));
		
		if(FMath::IsNearlyZero(LastDatas.Hit_Stop) == false) 
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-1f);
			UKismetSystemLibrary::K2_SetTimer(this, "Dilation_Return", LastDatas.Hit_Stop * 1e-1f,false);
		}
		Effect_Trans = trans;
		
	}


}

void ACAttackment_Melee::Seach_ComboIndex(EMouseBut but)
{
	while(Attack_Cnt < ComboRow.Num())
	{
		FWeaponData_Player temp = *(ComboTable->FindRow<FWeaponData_Player>(ComboRow[Attack_Cnt],ComboRow[Attack_Cnt].ToString()));
		if (temp.Input_But == but) 
		{
			ComboData = temp;
			Attack_Cnt++;
			return;
		}
		else
			Attack_Cnt++;
	}
	
	Attack_Cnt = 0 ; // 못찾을 경우
	
	while (Attack_Cnt < ComboRow.Num())
	{
		FWeaponData_Player temp = *(ComboTable->FindRow<FWeaponData_Player>(ComboRow[Attack_Cnt], ComboRow[Attack_Cnt].ToString()));
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


void ACAttackment_Melee::Dilation_Return()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), L_PushDatas[Attack_Cnt].Effect, Effect_Trans, true);
}

