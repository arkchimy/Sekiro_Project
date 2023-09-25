#include "CAnimNotifyState_Collision.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"
#include "Actors/GameActor.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"


//OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
//player = Cast<ACPlayer>(OwnerCharacter);
//enemy = Cast<ACEnemy>(OwnerCharacter);
//if (!!player)
//{
//	CombatComp = CHelpers::GetComponent<UCombatComponent>(player);
//
//	CombatComp->Player_AttackSound();
//	CombatComp->On_Collision();
//}
FString UCAnimNotifyState_Collision::GetNotifyName() const
{
	return "Collision";
}
void UCAnimNotifyState_Collision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	CheckNull(MeshComp);
	OwnerActor = Cast<AGameActor>(MeshComp->GetOwner());
	if (!!OwnerActor) 
	{
		CombatComp = CHelpers::GetComponent<UCombatComponent>(OwnerActor);
		if (!!CombatComp)
			CombatComp->On_Collision(AttackType, KnockBack_Power);
	}

	
}

void UCAnimNotifyState_Collision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	CheckNull(MeshComp);
	if (!!OwnerActor)
	{
		CombatComp = CHelpers::GetComponent<UCombatComponent>(OwnerActor);
		if (!!CombatComp)
			CombatComp->Off_Collision();
	}
	
}
