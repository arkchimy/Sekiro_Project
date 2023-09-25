#include "CAnimNotifyState_EnableCombo.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"

FString UCAnimNotifyState_EnableCombo::GetNotifyName() const
{
	return "EnableCombo";
}

void UCAnimNotifyState_EnableCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	CheckNull(MeshComp);
	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	
	if(!!OwnerCharacter)
	{
		CombatComponent = CHelpers::GetComponent<UCombatComponent>(OwnerCharacter);
		if (!!CombatComponent)
			CombatComponent->Enable_Combo();
	}
}

void UCAnimNotifyState_EnableCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	CheckNull(MeshComp);
	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (!!OwnerCharacter)
	{
		CombatComponent = CHelpers::GetComponent<UCombatComponent>(OwnerCharacter);
		if(!!CombatComponent)
			CombatComponent->Unable_Combo();
	}
}
