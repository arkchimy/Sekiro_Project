#include "Notifies/CAnimNotify_PlayAttackSound.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"


FString UCAnimNotify_PlayAttackSound::GetNotifyName() const
{
	return "AttackSound";
}
void UCAnimNotify_PlayAttackSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	player = Cast<ACPlayer>(OwnerCharacter);
	enemy = Cast<ACEnemy>(OwnerCharacter);
	if (!!player)
	{
		Combatcomp = CHelpers::GetComponent<UCombatComponent>(player);
		if (!!Combatcomp) 
		{
			Combatcomp->Player_AttackSound();
		}
	}

	else if (!!enemy)
	{
		Combatcomp = CHelpers::GetComponent<UCombatComponent>(enemy);

		if (!!Combatcomp) 
		{
			Combatcomp->Player_AttackSound();
		}
	}
}

