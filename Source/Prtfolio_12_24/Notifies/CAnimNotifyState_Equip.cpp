#include "CAnimNotifyState_Equip.h"
#include "Global.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"

FString UCAnimNotifyState_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UCAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCombatComponent* Combat = CHelpers::GetComponent<UCombatComponent>(MeshComp->GetOwner());

	
	CheckNull(Combat);

	Combat->Begin_Equip();
}

void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCombatComponent* Combat = CHelpers::GetComponent<UCombatComponent>(MeshComp->GetOwner());
	CheckNull(Combat);


	Combat->End_Equip();
	
}
