#include "CAnimNotifyState_UnEquip.h"
#include "Global.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"
#include "Actors/CThrowActors/CThrow_AttachDummy.h"
#include "Actors/CPlayer.h"

#include "Actors/CWeaponment.h"
#include "Actors/CEquipment.h"
#include "Actors/CAttackments/CAttackment.h"
FString UCAnimNotifyState_UnEquip::GetNotifyName_Implementation() const
{
	return "UnEquip";
}

void UCAnimNotifyState_UnEquip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCombatComponent* Combat = CHelpers::GetComponent<UCombatComponent>(MeshComp->GetOwner());
	CheckNull(Combat);

	//Attach Ç®±â
	Combat->Begin_UnEquip();
}

void UCAnimNotifyState_UnEquip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCombatComponent* Combat = CHelpers::GetComponent<UCombatComponent>(MeshComp->GetOwner());
	CheckNull(Combat);

	Combat->End_UnEquip();
	ECombat_Type temp = Combat->Get_SwapCombat();
	if(temp == ECombat_Type::Unarmed && !Combat->Swapping)
		Combat->ChangeType(ECombat_Type::Unarmed);
	if (Combat->Swapping)
		Combat->Swapping = false;
	if(temp != ECombat_Type::Unarmed)
	{
		CLog::Log("=====change swap");
		CLog::Log(int32(temp));
		Combat->ChangeType(temp);
		Combat->Equip();
		Combat->Set_SwapCombat(ECombat_Type::Unarmed);
		Combat->Swapping = true;
	}

	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (!!OwnerCharacter)
		player = Cast<ACPlayer>(OwnerCharacter);
	if (!!player)
		Dummy = player->Get_ThrowDummy();
	if (!!Dummy)
		Dummy->Set_Visible(false);
	
}
