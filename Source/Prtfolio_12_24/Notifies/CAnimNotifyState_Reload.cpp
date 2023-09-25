#include "Notifies/CAnimNotifyState_Reload.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"

#include "Actors/CThrowActors/CThrow_AttachDummy.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"

FString UCAnimNotifyState_Reload::GetNotifyName() const
{
	return "Reload";
}
void UCAnimNotifyState_Reload::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	if (!!MeshComp) 
	{
		OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
		player = Cast<ACPlayer>(MeshComp->GetOwner());
		enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	}
	if (!!player) 
	{
		Dummy = player->Get_ThrowDummy();
		Combatcomp = CHelpers::GetComponent<UCombatComponent>(player);

		if (!!Dummy && !!Combatcomp)
		{
			Combatcomp->Reload_Arraw();
			Dummy->Set_Visible(false);

		}
	}
	else if(!!enemy)
	{
		Dummy = enemy->Get_ThrowDummy();
		Combatcomp = CHelpers::GetComponent<UCombatComponent>(enemy);

		if (!!Dummy && !!Combatcomp)
		{

			Combatcomp->Reload_Arraw();
			Dummy->Set_Visible(false);

		}
	}

	
	
}		

void UCAnimNotifyState_Reload::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	CheckNull(player);
	CheckNull(Dummy);
	if (!!MeshComp) 
	{
		if (!!Dummy)
			Dummy->Set_Visible(true);
		player = Cast<ACPlayer>(MeshComp->GetOwner());
		if (!!player)
		{
			Dummy = player->Get_ThrowDummy();
			Combatcomp = CHelpers::GetComponent<UCombatComponent>(player);
			if(!!Combatcomp)
				Combatcomp->End_Attack();
		}
	}
}
