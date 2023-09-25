#include "CombatComponent.h"
#include "CCombatData_Player.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actors/CPlayer.h"

#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraShakeBase.h"

#include "Actors/CWeaponment.h"
#include "Actors/CEquipment.h"
#include "Actors/CAttackments/CAttackment.h"


UCombatComponent::UCombatComponent()
{
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);


	for (int i = 0; i < int32(ECombat_Type::Max); i++)
	{
		if (!!Datas[i])
		{
			Datas[i]->BeginPlay(OwnerCharacter);

		}
	}
}



void UCombatComponent::EquipSlot_Equip(int idx, UCCombatData* CombatData)
{
	if(!!Datas[idx])
	{	// Abort 
		if (Datas[idx] == UnArmed_Combat)
		{	// Datas[Sheath] ==  UnArmed Case
			
		}
		Datas[idx] = CombatData; 
		Datas[idx]->BeginPlay(OwnerCharacter);
	}
	else 
	{
		Datas[idx] = CombatData;
		Datas[idx]->BeginPlay(OwnerCharacter);
	}
}
void UCombatComponent::EquipSlot_UnEquip(int idx)
{
	if (!!Datas[idx])
	{	// Abort 
		Datas[idx]->DestroyActor();
		if(idx == 1)
		{
			Datas[idx] = UnArmed_Combat;
			Datas[idx]->BeginPlay(OwnerCharacter);
		}
		
	}
	
}

void UCombatComponent::Play_Action(const EActionBut but)
{
	Datas[int32(Combat)]->Play_Action(but);
}

void UCombatComponent::Jump_End()
{
	Datas[int32(Combat)]->Jump_End();
}

void UCombatComponent::ChangeType(ECombat_Type type)
{
	Combat = type;
	
}

void UCombatComponent::FPS_Weapon()
{
	if (!!Datas[int32(Combat)]) 
	{

		Weaponment = Datas[int32(Combat)]->Get_Weaponment();
		if (!!Weaponment)
		{
			Weaponment->FPS_Weapon();
		}
		else
		{
			CLog::Print(" Weaponment is Null");
		}
	}
}

void UCombatComponent::TPS_Weapon()
{
	for(int i = 0; i < 5; i++)
	{
		if (!!Datas[i]) 
		{
			Weaponment = Datas[i]->Get_Weaponment();
			if (!!Weaponment)
				Weaponment->TPS_Weapon();
			else
				CLog::Print("No has Weaponment");
		}
	}
	
		
}

ECamera_Sight UCombatComponent::Get_CombatSight(ECombat_Type type)
{
	if(!!Datas[int32(type)]) 
		return Datas[int32(type)]->Get_Sight(); 
	return ECamera_Sight::FPS;
}

void UCombatComponent::Equip() // Unarmed가 Equip이 호출이 안되다 보니까 attackment 가 전에있던 무기를 가져오게됌
{
	if (!!Datas[int32(Combat)]) 
	{
		equipment = Datas[int32(Combat)]->Get_Equipment();
		Weaponment = Datas[int32(Combat)]->Get_Weaponment();
		attackment = Datas[int32(Combat)]->Get_Attackment();

		if (!!equipment)
		{
			equipment->Equip();
		}
	}
	
}

void UCombatComponent::Begin_Equip()
{
	if (!!equipment)
		equipment->Begin_Equip();
}

void UCombatComponent::End_Equip()
{
	if(!!equipment)
		equipment->End_Equip();

}


void UCombatComponent::UnEquip()
{
	if (!!Datas[int32(Combat)])
	{
		equipment = Datas[int32(Combat)]->Get_Equipment();
		equipment->Unequip();
	}
}

void UCombatComponent::Begin_UnEquip()
{
	
	equipment->Begin_UnEquip();
}
void UCombatComponent::End_UnEquip()
{
	equipment->End_UnEquip();
}

void UCombatComponent::On_Collision(EAttackTag_Type AttackType, float Knoback_Power)
{
	if (!!Datas[int32(Combat)])
	{
		Weaponment->On_Collision(AttackType, Knoback_Power);
	}
}

void UCombatComponent::Off_Collision()
{
	if (!!Datas[int32(Combat)])
	{
		Weaponment->Off_Collision();
	}
}

void UCombatComponent::On_Parrying()
{
	if (!!Datas[int32(Combat)])
	{
		Weaponment->On_Parrying();
	}
}

void UCombatComponent::Off_Parrying()
{
	if (!!Datas[int32(Combat)])
	{
		Weaponment->Off_Parrying();
	}
}

void UCombatComponent::ParryingMontage(EDirection direction)
{// 패링에 성공한 Montage
	if (!!Datas[int32(Combat)])
	{
		Datas[int32(Combat)]->ParryingMontage(direction);
	}
	
}

void UCombatComponent::BeParriedMontage(EDirection direction, ACharacter* ActionActor)
{// 패링 당한 Montage
	if (!!Datas[int32(Combat)])
	{
		Datas[int32(Combat)]->BeParriedMontage(direction, ActionActor);
		//CLog::Print("Parried Called");
	}
	
	
}
UAnimMontage* UCombatComponent::Get_BeParriedMontage(EDirection direction)
{// 패링 당한 Montage
	/*if (!!Datas[int32(Combat)])
	{
		Datas[int32(Combat)]->BeParriedMontage(direction);
	}*/
	return nullptr;
}

void UCombatComponent::Monster_Attack()
{
	if (!!Datas[int32(Combat)])
	{	
		Datas[int32(Combat)]->Set_OwnerCharacter(OwnerCharacter);
		attackment = Datas[int32(Combat)]->Get_Attackment();
		//
		if (!!attackment) 
		{
			attackment->Set_OwnerCharacter(OwnerCharacter);
			attackment->Monster_Attack();
		}
		else
			CLog::Print("attackment is Null");
	}
}
void UCombatComponent::Do_Parrying()
{
	if (!!Datas[int32(Combat)])
	{
		if (!!attackment)
			attackment->Do_Parrying();
	}
}
void UCombatComponent::SuddenAttack()
{
	if (!!Datas[int32(Combat)])
	{
		if (!!attackment)
			attackment->SuddenAttack();
	}
}

void UCombatComponent::L_Attack()
{
	if (!!Datas[int32(Combat)])
	{
		if (!!attackment)
			attackment->Do_LAttack();
	}
}
void UCombatComponent::R_Attack()
{
	if (!!Datas[int32(Combat)])
	{
		if(!!attackment)
			attackment->Do_RAttack();
	}
}

void UCombatComponent::T_Attack()
{
	if (!!Datas[int32(Combat)])
	{
		if (!!attackment)
			attackment->T_Attack();
		
	}

}

void UCombatComponent::T_AttackRelease()
{
	if (!!Datas[int32(Combat)])
	{
		attackment->T_AttackRelease();

	}
}

void UCombatComponent::L_AttackRelease()
{
	if (!!Datas[int32(Combat)])
	{
		if (!!attackment)
			attackment->L_AttackRelease();
	}
}
void UCombatComponent::R_AttackRelease()
{
	if (!!Datas[int32(Combat)])
	{
		if(!!attackment)
			attackment->R_AttackRelease();
	}
}



void UCombatComponent::Begin_Attack()
{
	//ACAttackment* attackment = Datas[int32(Combat)]->Get_Attackment();
	attackment->Begin_Attack();
}
void UCombatComponent::End_Attack()
{
	//ACAttackment* attackment = Datas[int32(Combat)]->Get_Attackment();
	if (!!attackment)
		attackment->End_Attack();
	else 
	{
		CLog::Print("has not Attackment");

	}
}



void UCombatComponent::Enable_Combo()
{
	//ACAttackment* attackment = Datas[int32(Combat)]->Get_Attackment();
	attackment->Enable_Combo();

}

void UCombatComponent::Unable_Combo()
{
	//ACAttackment* attackment = Datas[int32(Combat)]->Get_Attackment();
	attackment->Unable_Combo();
}

void UCombatComponent::NextAttack_Play()
{
	//ACAttackment* attackment = Datas[int32(Combat)]->Get_Attackment();
	attackment->NextAttack_Play();
}

void UCombatComponent::Resiste_PlayMontage()
{
	attackment->Resiste_PlayMontage();
}

void UCombatComponent::Reload_Arraw()
{
	//ACAttackment* attackment = Datas[int32(Combat)]->Get_Attackment();
	if(!!attackment)
		attackment->Reload_Arraw();
	
}
void UCombatComponent::Pressed_But()
{
	
	if (!!attackment)
		attackment->Pressed_But();

}




void UCombatComponent::Player_AttackSound()
{
	if (!!attackment)
		attackment->Player_AttackSound();
}

void UCombatComponent::Slot_Swap(ACWeaponment * Swap_Weapon)
{
	Datas[int32(Combat)]->Slot_Swap(Swap_Weapon);
}

