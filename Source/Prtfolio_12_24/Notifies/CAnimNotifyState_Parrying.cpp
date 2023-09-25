#include "Notifies/CAnimNotifyState_Parrying.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actors/GameActor.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"


FString UCAnimNotifyState_Parrying::GetNotifyName() const
{
	return "Parrying_Mode";
}
void UCAnimNotifyState_Parrying::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	CheckNull(MeshComp);
	OwnerCharacter = Cast<AGameActor>(MeshComp->GetOwner());

	if (!!OwnerCharacter)
	{
		OwnerCharacter->Set_ParryingDirection(Parrying_Direction);
		CombatComp = CHelpers::GetComponent<UCombatComponent>(OwnerCharacter);
		if(!!CombatComp)
			CombatComp->On_Parrying();
	}

}

void UCAnimNotifyState_Parrying::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	CheckNull(MeshComp);
	CheckNull(OwnerCharacter);
	CheckNull(CombatComp);
	OwnerCharacter->Set_ParryingDirection(EDirection::Max);
	if (!!OwnerCharacter && !!CombatComp)
	{

		CombatComp->Off_Collision();
		CombatComp->Off_Parrying();
	}
	
}
