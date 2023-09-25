#include "CAnimNotify_Pressed.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"

FString UCAnimNotify_Pressed::GetNotifyName() const
{
	return "Pressed";
}
void UCAnimNotify_Pressed::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
	CheckNull(MeshComp);
	
	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());

	if (!!OwnerCharacter)
	{
		Combatcomp = CHelpers::GetComponent<UCombatComponent>(OwnerCharacter);
		Combatcomp->Pressed_But();
	}
}
