#include "Notifies/Enemies/CEnemyLookAt_Target.h"

#include "Global.h"
#include "Actors/GameActor.h"
#include "ActorComponents/State_Component/CStateComponent.h"

FString UCEnemyLookAt_Target::GetNotifyName() const
{
	return "LookAt_Player";
}
void UCEnemyLookAt_Target::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(OwnerCharacter);

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(State);

	State->Set_InteractionMode();
}

