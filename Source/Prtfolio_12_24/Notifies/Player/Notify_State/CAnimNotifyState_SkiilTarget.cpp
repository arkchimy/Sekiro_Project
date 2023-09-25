#include "CAnimNotifyState_SkiilTarget.h"
#include "Global.h"
#include "ActorComponents/CActorComponent_Target.h"

FString UCAnimNotifyState_SkiilTarget::GetNotifyName_Implementation() const
{
	return "Skill_Target";
}

void UCAnimNotifyState_SkiilTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	CheckNull(MeshComp->GetOwner());

	TargetComp = CHelpers::GetComponent<UCActorComponent_Target>(MeshComp->GetOwner());
	CheckNull(TargetComp);
	TargetComp->Skill_Look_Start();
}

void UCAnimNotifyState_SkiilTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
	CheckNull(MeshComp->GetOwner());
	TargetComp = CHelpers::GetComponent<UCActorComponent_Target>(MeshComp->GetOwner());
	CheckNull(TargetComp);
	TargetComp->Skill_Look_End();

}
