#include "Notifies/Enemies/CEnemyNotify_EndAction.h"

#include "Global.h"
#include "Actors/GameActor.h"
#include "ActorComponents/State_Component/CStateComponent.h"


FString UCEnemyNotify_EndAction::GetNotifyName() const
{
	return "Enemy_ActionEnd";
}
void UCEnemyNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(OwnerCharacter);

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(State);

	State->Set_IdleMode();
}

