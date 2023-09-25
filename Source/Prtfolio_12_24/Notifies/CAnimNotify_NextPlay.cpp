#include "CAnimNotify_NextPlay.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"

FString UCAnimNotify_NextPlay::GetNotifyName() const
{
	return "NextPlay";
}
void UCAnimNotify_NextPlay::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	Player = Cast<ACPlayer>(OwnerCharacter);
	
	if(!!Player)
	{
		Combatcomp = CHelpers::GetComponent<UCombatComponent>(Player);
		Combatcomp->NextAttack_Play();
		
	}
}
