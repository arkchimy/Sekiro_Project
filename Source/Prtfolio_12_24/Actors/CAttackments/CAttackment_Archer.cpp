#include "Actors/CAttackments/CAttackment_Archer.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"

#include "Actors/CThrowActors/CThrowItem.h"
#include "Actors/CThrowActors/CThrow_AttachDummy.h"
#include "Actors/CThrowActors/CThrow_ManagerActor.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

#include "ActorComponents/State_Component/CStateComponent.h"
#include "ActorComponents/CStatusComponent.h"

void ACAttackment_Archer::BeginPlay()
{

	Super::BeginPlay();
}

void ACAttackment_Archer::Enable_Combo()
{
	Nextable = true;
}
void ACAttackment_Archer::Unable_Combo()
{
	Nextable = false;

}

void ACAttackment_Archer::NextAttack_Play()
{
	CheckFalse(NextActtack);

	NextActtack = false;
	State->Set_ActionMode();
	Montage_Play(ComboData); // 다음 공격이 들어있음.

	if (ComboData.LastAttack) // 마지막 공격이라면
	{
		Attack_Cnt = 0;
		ComboData = *(ComboTable->FindRow<FFPSWeaponData>(ComboRow[Attack_Cnt], ComboRow[Attack_Cnt].ToString())); // 화살 충전하기 Reload Montage
	}

	LastDatas = ComboData;
}

void ACAttackment_Archer::Button_Event(EMouseBut input)
{

	if (input == EMouseBut::Left)
	{

		if (!R_push_Chk)  // R버튼이  push 중이 아닐떄. 
		{
			if (State->IsIdleState()) // 공격의 시작
			{
				L_push_Chk = true;
				Seach_ComboIndex(input); //Attack_Cnt Init 
				State->Set_ActionMode();

				LastDatas = ComboData;// Hit Stop 을 적용하려고 index 저장 개념
				Montage_Play(ComboData);

				if (!ComboData.bCanMove)
					State->Set_UnMove();

			}
			else if (State->IsRush() && Nextable) // Rush 공격
			{
				CheckFalse(Nextable);
				Seach_ComboIndex(EMouseBut::Shift_Attack); //Attack_Cnt Init 

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
	if (input == EMouseBut::Right)
	{
		if (State->IsIdleState()) // 화살 stack 
		{
			R_push_Chk = true;

			Seach_ComboIndex(input); //Attack_Cnt Init 
			State->Set_ActionMode();
			LastDatas = ComboData;
			Montage_Play(ComboData);

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


	}

}

void ACAttackment_Archer::Montage_Play(FFPSWeaponData input)
{
	OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(input.Data.Montage, input.Data.PlayRatio);
	player->Get_FP_Mesh()->GetAnimInstance()->Montage_Play(input.FPS_Montage, input.FPS_PlayRatio);
	
}
void ACAttackment_Archer::Montage_Play(FFPSWeaponData input, FName Section)
{
	OwnerCharacter->PlayAnimMontage(input.Data.Montage, input.Data.PlayRatio, Section);

}



void ACAttackment_Archer::Do_LAttack()
{

	CheckTrue(State->IsActionState());

	Button_Event(EMouseBut::Left);
	UKismetSystemLibrary::K2_SetTimer(this, "PressedAction", 0.01f, false);
	UKismetSystemLibrary::K2_SetTimer(this, "DrawDebug_Arc", 0.01f, false);
	UKismetSystemLibrary::K2_SetTimer(this, "ZoomAction", 0.01f, true);
	player->Archer_CrossHair(true);

	if (WeaponNiagara_Delegate.IsBound() && player->Get_WheelCnt() != 1)
		WeaponNiagara_Delegate.Broadcast(true);

}
void ACAttackment_Archer::Do_RAttack()
{
	Button_Event(EMouseBut::Right);
	if (!!player) 
	{
		Dummy = player->Get_ThrowDummy();
		Dummy->Trigger_TagChk(FName("Fire"));
	}
}

void ACAttackment_Archer::L_AttackRelease()
{
	CheckTrue(State->IsAbsolutState	());
	CheckFalse(L_push_Chk);

	L_push_Chk = false;
	UKismetSystemLibrary::K2_ClearTimer(this, "PressedAction");

	if (WeaponNiagara_Delegate.IsBound())
		WeaponNiagara_Delegate.Broadcast(false);  // 이거일단 Arrow의 불이 안꺼짐

	//OwnerCharacter->StopAnimMontage();
	//player->Get_FP_Mesh()->GetAnimInstance()->Montage_Stop(0.1f);
	player->Archer_CrossHair(false);
	State->Set_IdleMode();

	player->WheelCnt(1);

	FVector start, end, direction;
	player->GetLocationAndDirection(start, end, direction);

	if (PressVal > 3.f)
	{
		if (!!causer)
		{

			causer->Trail_Visible(false);
			FName state = Dummy->Get_State();

			if (state != "None") // 
				causer->Set_State(state);
			else
				causer->Set_State(FName("Normal"));
			causer->Set_Pressed(PressVal * 3, OwnerCharacter);

			Reload_Montage = causer->Get_ReloadMontage();

		}

		player->Get_FP_Mesh()->GetAnimInstance()->Montage_Play(Reload_Montage.FP_Montage, Reload_Montage.FP_PlayRatio);
	}
	
	
	PressVal = 0;

	UKismetSystemLibrary::K2_ClearTimer(this, "ZoomAction");

	ZoomVal = 90;
	player->GetCamera()->SetFieldOfView(ZoomVal);
	UKismetSystemLibrary::K2_ClearTimer(this, "DrawDebug_Arc");
	if (WeaponNiagara_Delegate.IsBound())
		WeaponNiagara_Delegate.Broadcast(false);
}



void ACAttackment_Archer::R_AttackRelease()
{
	R_push_Chk = false;
}

void ACAttackment_Archer::T_Attack()
{
	//Attack_Cnt = 0;
	//State->Set_AbsolutMode();
	//State->Set_UnMove();

	//OwnerCharacter->StopAnimMontage();
	//player->Get_FP_Mesh()->GetAnimInstance()->Montage_Pause();

	//OwnerCharacter->PlayAnimMontage(Special_AttackDatas[Attack_Cnt].Try_Montage, Special_AttackDatas[Attack_Cnt].Try_PlayRatio);
	//
	//// 카메라 전환.
	//player->Set_View_Fallow_Camera();

	//if(!!causer)
	//	causer->Set_Throwing(false); // 가지고있던거 발사 전으로 돌리고.
	//player->WheelCnt(ThrowItem_Skill);
	
}

void ACAttackment_Archer::T_AttackRelease()
{
	
	//UKismetSystemLibrary::K2_ClearTimer(this, "Reload_Arraw_Loop");
	//State->Set_IdleMode();
	//player->WheelCnt(1);
	//Reload_Arraw();
}

void ACAttackment_Archer::Begin_Attack()
{
	
}

void ACAttackment_Archer::End_Attack()
{
	//CheckTrue(NextActtack); // 다음 공격으로 될경우 Idle 로 바꾸지않음.
	CheckTrue(State->IsAbsolutState());
	State->Set_IdleMode();
	Attack_Cnt = 0;
	Nextable = false;

}

void ACAttackment_Archer::OnBeginOverlap(const FHitResult& Hit,float Power, AActor* Causer)
{
	FDamageEvent Event_Type;
	AController* controll = player->GetController();
	if (!!player)
	{
		controll = player->GetController();
		ACEnemy* target = Cast<ACEnemy>(Hit.Actor);
		
		if (!!target) 
		{
			target->Set_HitBone(Hit.BoneName);
			//target->Set_Attack_Tag(ComboData.Attack_Tag);
		}
	}
	else if(!!enemy)
	{
		controll = enemy->GetController();
	}

	Hit.Actor->TakeDamage(Power, Event_Type, controll, Causer);
	//causer->ThrowBeginOverlap.RemoveAll(this);
}


void ACAttackment_Archer::ZoomAction()
{
	if (ZoomVal > 60)
		ZoomVal -= 1.0f;
	else
		ZoomVal = 60;
	
	player->GetCamera()->SetFieldOfView(ZoomVal);
	UKismetSystemLibrary::K2_SetTimer(this, "ZoomAction", 0.01f, true);
}

void ACAttackment_Archer::PressedAction()
{
	PressVal += 0.15f;
	
	UKismetSystemLibrary::K2_SetTimer(this, "PressedAction", 0.01f, true);
	
}

void ACAttackment_Archer::DrawDebug_Arc()
{
	
	//UKismetSystemLibrary::K2_SetTimer(this, "DrawDebug_Arc", 0.5f,true);
}

void ACAttackment_Archer::Reload_Arraw()
{
	if (!State->IsAbsolutState()) 
	{
		State->Set_ActionMode();
		
	}

	if(!!player)
		causer = player->Get_ThrowManager()->Find_ThrowActor(1);
	
	if (!!causer) 
	{
		if (causer->ThrowComponentHit.IsBound())
			causer->ThrowComponentHit.RemoveDynamic(this, &ACAttackment_Archer::OnBeginOverlap);
		causer->ThrowComponentHit.AddDynamic(this, &ACAttackment_Archer::OnBeginOverlap);
		
	}
	if (!!player)
		Dummy = player->Get_ThrowDummy();
	else if (!!enemy)
		Dummy = enemy->Get_ThrowDummy();

	if (!!Dummy && !!causer)
	{
		Dummy->Set_StaticMesh(causer->Get_ThrowMesh());
		Dummy->SetActorRelativeRotation(FQuat(causer->Edit.Rot));
		Dummy->SetActorScale3D(causer->Edit.Scale);
	}
	CheckFalse(State->IsAbsolutState()); //궁극기 모드라면
	
	
	if (WeaponNiagara_Delegate.IsBound())
		WeaponNiagara_Delegate.Broadcast(true);


	FVector start, end, direction;
	player->GetLocationAndDirection(start, end, direction);

	if (!!causer)
	{

		causer->Trail_Visible(false);
		FName state = Dummy->Get_State();
		if (state != "None") // 
			causer->Set_State(state);
		else
			causer->Set_State(FName("Normal"));
		causer->Set_Pressed(40, OwnerCharacter);

	}

	UKismetSystemLibrary::K2_SetTimer(this, "Reload_Arraw_Loop", 0.2f, false);
	

}

void ACAttackment_Archer::Reload_Arraw_Loop()
{
	Reload_Arraw();
	if (shake != NULL)
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(shake);
}

void ACAttackment_Archer::Seach_ComboIndex(EMouseBut but)
{
	while (Attack_Cnt < ComboRow.Num())
	{
		FFPSWeaponData temp = *(ComboTable->FindRow<FFPSWeaponData>(ComboRow[Attack_Cnt], ComboRow[Attack_Cnt].ToString()));
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
		FFPSWeaponData temp = *(ComboTable->FindRow<FFPSWeaponData>(ComboRow[Attack_Cnt], ComboRow[Attack_Cnt].ToString()));
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