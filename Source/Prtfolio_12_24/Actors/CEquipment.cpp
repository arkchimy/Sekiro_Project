#include "CEquipment.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"
#include "GameFramework/Character.h"

#include "ActorComponents/State_Component/CStateComponent.h"
#include "ActorComponents/CStatusComponent.h"
#include "Components/SkeletalMeshComponent.h"
ACEquipment::ACEquipment()
{


}

void ACEquipment::BeginPlay()
{
	

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	player = Cast<ACPlayer>(OwnerCharacter);

	enemy = Cast<ACEnemy>(OwnerCharacter);
	if (!!player) 
	{
		FP_Mesh = player->Get_FP_Mesh();
		
	}
	
	Super::BeginPlay();

}

//Notify 로 활용하기 위해서 Delegate로 선언함
void ACEquipment::Equip_Implementation()
{
	
	
	enemy = Cast<ACEnemy>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	
	CheckFalse(State->IsIdleState());


	State->Set_EquipMode();
	
	OwnerCharacter->PlayAnimMontage(Equip_Montage.Montage, Equip_Montage.PlayRatio);
	if (!!player) 
	{
		player->Player_Move(Equip_Montage.bCanMove);
		FP_Mesh->GetAnimInstance()->Montage_Play(Equip_Montage.FP_Montage, Equip_Montage.FP_PlayRatio);
		
	}
	
}

void ACEquipment::Begin_Equip_Implementation()
{
	
	if (OnEquipmentDelegate.IsBound()) 
	{
		OnEquipmentDelegate.Broadcast(Draw_Socket);
		
		//OnEquipmentDelegate.Clear();
	}
}

void ACEquipment::End_Equip_Implementation()
{
	State->Set_IdleMode();
	if (Cross_Hair.IsBound()) 
	{
		Cross_Hair.Broadcast();
		
	}
	
}
//Notify 로 활용하기 위해서 Delegate로 선언함
void ACEquipment::Unequip_Implementation()
{
	

	CheckFalse(State->IsIdleState());
	State->Set_EquipMode();

	
	OwnerCharacter->PlayAnimMontage(UnEquip_Montage.Montage, UnEquip_Montage.PlayRatio);
	if(!!player)
		FP_Mesh->GetAnimInstance()->Montage_Play(UnEquip_Montage.FP_Montage, UnEquip_Montage.FP_PlayRatio);
	player->Player_Move(UnEquip_Montage.bCanMove);
	CLog::Log("UnEquip_exe");
	
}

void ACEquipment::Begin_UnEquip_Implementation()
{
	if (OnUnequipmentDelegate.IsBound()) 
	{
		OnUnequipmentDelegate.Broadcast(Sheathe_Socket);
		//OnUnequipmentDelegate.Clear();
	}
}
void ACEquipment::End_UnEquip_Implementation()
{
	//if (OnUnequipmentDelegate.IsBound())
	//	OnUnequipmentDelegate.Broadcast();

	State->Set_IdleMode();
}
