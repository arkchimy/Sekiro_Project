#include "CAnimNotify_EndAttack.h"
#include "Global.h"


#include "Actors/GameActor.h"

#include "ActorComponents/Combat_Component/CombatComponent.h"


FString UCAnimNotify_EndAttack::GetNotifyName() const
{
	return "End_Attack";
}
void UCAnimNotify_EndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());

	if (!!OwnerCharacter)
	{
		Combatcomp = CHelpers::GetComponent<UCombatComponent>(OwnerCharacter);
		if (!!Combatcomp)
			Combatcomp->End_Attack();
	}
	
	
}

