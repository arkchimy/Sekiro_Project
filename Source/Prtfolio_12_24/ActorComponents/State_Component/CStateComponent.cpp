#include "CStateComponent.h"
#include "Global.h"
#include "Actors/CWeaponment.h"
#include "Actors/CThrowActors/CThrowItem.h"
#include "Actors/CEnemy.h"
#include "Actors/CPlayer.h"

#include "Engine/DataTable.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Sound/SoundCue.h"
#include "GameFramework/Character.h"

UCStateComponent::UCStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	enemy = Cast<ACEnemy>(OwnerCharacter);
}

void UCStateComponent::Set_IdleMode()
{
	state = EStateType::Idle; 
	bRush = false;
	Set_Move();
}

void UCStateComponent::Set_ActionMode()
{
	CheckTrue(state == EStateType::Death); // 죽었을경우엔 안돼
	state = EStateType::Action;
}

void UCStateComponent::Set_StunMode()
{
	state = EStateType::Stun;
}

void UCStateComponent::Set_HitMode()
{
	state = EStateType::Hit;
}

void UCStateComponent::Set_DeathMode()
{
	//CLog::Print("DeathMode");
	state = EStateType::Death;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathMontage.Hit_Sound, GetOwner()->GetActorLocation());
	if (!!DeathMontage.Montage)
		OwnerCharacter->PlayAnimMontage(DeathMontage.Montage, DeathMontage.PlayRatio,"Loop");
	else 
	{
		OwnerCharacter->GetMesh()->SetSimulatePhysics(true);
	}
	
	bcanMove = false;
}

void UCStateComponent::Set_EquipMode()
{
	state = EStateType::Equip;
}

void UCStateComponent::Set_BlockMode()
{
	state = EStateType::Block;
}

void UCStateComponent::Set_InteractionMode()
{
	state = EStateType::Interaction;
}

void UCStateComponent::Set_AbsolutMode()
{
	state = EStateType::Absolut;
}

bool UCStateComponent::CanMove()
{
	return bcanMove;
}

void UCStateComponent::Hiteed_Direction(AActor* Causer, EDirection* direction)
{
	float dot_Result;
	FVector Cross_Result;
	ACThrowItem* Throw_Item = Cast<ACThrowItem>(Causer);
	if (!!Throw_Item) // 화살의 스태틱 메쉬 의 Rot떄문에 임의적으로 Edit_rot를 추가계산
	{
		FQuat rot = FQuat(Causer->GetActorRotation() - Throw_Item->Edit.Rot);

		dot_Result = UKismetMathLibrary::Dot_VectorVector(GetOwner()->GetActorForwardVector(), rot.GetForwardVector());
		Cross_Result = UKismetMathLibrary::Cross_VectorVector(GetOwner()->GetActorForwardVector(), rot.GetForwardVector());
		
	}
	else 
	{
		dot_Result = UKismetMathLibrary::Dot_VectorVector(GetOwner()->GetActorForwardVector(), Causer->GetActorForwardVector());
		Cross_Result = UKismetMathLibrary::Cross_VectorVector(GetOwner()->GetActorForwardVector(), Causer->GetActorForwardVector());
	}

	bool Forward_Chk, Right_Chk;

	Forward_Chk = dot_Result < 0 ? true : false; // 정면에서 맞음
	
	if( -0.5f < Cross_Result.Z  && Cross_Result.Z < 0.5f)
	{
		/*if (Forward_Chk)
			CLog::Print("Front");
		else
			CLog::Print("Back");*/
		*direction = Forward_Chk ? EDirection::Front : EDirection::Back;
	}
	else 
	{
		Right_Chk = 0 < Cross_Result.Z ? true : false;
		/*if (Right_Chk)
			CLog::Print("Left");
		else
			CLog::Print("Right");*/
		*direction = Right_Chk ? EDirection::Left : EDirection::Right;
	}
	
}

void UCStateComponent::Hitted_PlayMontage(AActor* causer, EAttackTag_Type attack_tag)//FName
{


	ACWeaponment* weapon = Cast<ACWeaponment>(causer);
	ACThrowItem* Throw_Item = Cast<ACThrowItem>(causer);

	EDirection direction = EDirection::Back;
	//EAttackTag_Type attack_tag = EAttackTag_Type::Normal;

	
	if (!!Throw_Item) // 투척 공격의 경우
	{
		Hiteed_Direction(Throw_Item, &direction);
	}
	else if (!!weapon) // 근접 공격의 경우
	{
		Hiteed_Direction(weapon->GetOwner(), &direction);
	}

	if (GetOwner() == causer)  // 도트데미지
	{
		CheckTrue(state == EStateType::Absolut);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Attached_Montage.Hit_Sound, GetOwner()->GetActorLocation());
		OwnerCharacter->PlayAnimMontage(Attached_Montage.Montage, Attached_Montage.PlayRatio);
	}
	else
	{
		for (FName boneName : WeekBone)// WeekHitted Chk
		{
			if (Hit_Bone.ToString().Contains((boneName.ToString())))
			{
		
				Play_Montage(direction, boneName); // 약점 Montage 실행
				return;
			}
		}
		
		Play_Montage(direction, attack_tag);	// 약점 외 Montage 실행
		if (!!enemy)
			enemy->Set_Attack_Tag(EAttackTag_Type::Normal); // 초기화
	}
}

void UCStateComponent::Play_Montage(const EDirection direction,const FName BoneName)
{
	// 약점에 맞은경우
	Hit_Bone = "None";
	DataTable_Row = DT_WeekHitData->GetRowNames();
	
	for (int i = 0; i < DataTable_Row.Num(); i++) 
	{
		WeekHitData = *(DT_WeekHitData->FindRow<FWeekHit_Data>(DataTable_Row[i], DataTable_Row[i].ToString()));
		for (FName montage_bone : WeekHitData.WeekBone)
		{
			if (BoneName == montage_bone && WeekHitData.direction == direction)
			{	// 약점임과 동시에 방향이 맞을 경우
				//UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeekHitData.Hit_Sound, GetOwner()->GetActorLocation());
				OwnerCharacter->PlayAnimMontage(WeekHitData.Montage, WeekHitData.PlayRatio, WeekHitData.StartSection);
				bcanMove = WeekHitData.bCanMove;
				state = EStateType::Stun;
				return;
			}
		}
		
	}

	
}

void UCStateComponent::Play_Montage(const EDirection direction, const EAttackTag_Type attack_tag)
{
	// 약점 공격이 아닌 일반 적으로 공격 맞았을때
	
	CheckNull(DT_HitData);
	DataTable_Row = DT_HitData->GetRowNames();
	
	if (state == EStateType::Fly)
	{	// 에어본 중인지
		for (int i = 0; i < DataTable_Row.Num(); i++)
		{
			HitData = *(DT_HitData->FindRow<FHit_Data>(DataTable_Row[i], DataTable_Row[i].ToString()));
			
			if (HitData.state == state)
			{
				// 현재 Montage가 출력 중이라면
				OwnerCharacter->StopAnimMontage(OwnerCharacter->GetCurrentMontage());
				OwnerCharacter->PlayAnimMontage(HitData.Montage, HitData.PlayRatio);
				bcanMove = HitData.bCanMove;
				
				return;
			}
			//}
		}
		
	}
	CheckTrue(state == EStateType::Fly); 
	// 에어본 상태라면 Return;
	if((direction == EDirection::Front || direction == EDirection::Left || direction == EDirection::Right) && IsBlockState())
	{
		//정면 공격 + Block 상태일때
		ACPlayer* OwnerGameActor = Cast<ACPlayer>(OwnerCharacter);
		if (!!OwnerGameActor) 
		{
			for (int i = 0; i < DataTable_Row.Num(); i++)
			{
				HitData = *(DT_HitData->FindRow<FHit_Data>(DataTable_Row[i], DataTable_Row[i].ToString()));
				if (attack_tag == EAttackTag_Type::Groggy)
				{ // Guard Breaker
					if (HitData.Tag == attack_tag && HitData.direction == direction)
					{
						OwnerCharacter->PlayAnimMontage(HitData.Montage, HitData.PlayRatio, HitData.StartSection);
						bcanMove = HitData.bCanMove;
						return;
					}
				}
			}
			OwnerGameActor->Block_Success();
		}
	}
	else 
	{
		
		for (int i = 0; i < DataTable_Row.Num(); i++)
		{
			HitData = *(DT_HitData->FindRow<FHit_Data>(DataTable_Row[i], DataTable_Row[i].ToString()));
			if (IsActionState())
			{	// 현재 공격을 하는 중이였다면 Idle로 취급하기.
				if (HitData.Tag == attack_tag && HitData.direction == direction && HitData.state == EStateType::Idle)
				{
					OwnerCharacter->PlayAnimMontage(HitData.Montage, HitData.PlayRatio, HitData.StartSection);
					bcanMove = HitData.bCanMove;
					if (attack_tag == EAttackTag_Type::AirBone)
						state = EStateType::Fly; // Airbone 상태
					return;
				}
			}
			else if (HitData.Tag == attack_tag && HitData.direction == direction && HitData.state == state)
			{
				/*
					Attacker의 attack 타입과 일치 & 방향도 일치  & Owner의 현재 상태 일치
				*/
				//UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitData.Hit_Sound, GetOwner()->GetActorLocation());
				OwnerCharacter->PlayAnimMontage(HitData.Montage, HitData.PlayRatio, HitData.StartSection);
				bcanMove = HitData.bCanMove;
				if (attack_tag == EAttackTag_Type::AirBone)
					state = EStateType::Fly; // Airbone 상태
				return;
			}
		}
		//CLog::Print("No matching WeekWord found in DataTable");
	}
}

void UCStateComponent::Play_FacedMontage() 
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FacedMontage.Hit_Sound, GetOwner()->GetActorLocation());
	if (!!FacedMontage.Montage) 
	{
		OwnerCharacter->PlayAnimMontage(FacedMontage.Montage, FacedMontage.PlayRatio);
		bcanMove = FacedMontage.bCanMove;
	}
}
