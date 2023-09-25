#include "Notifies/CAnimNotifyState_weak.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actors/GameActor.h"


FString UCAnimNotifyState_weak::GetNotifyName() const
{
	return "Counterable";
}
void UCAnimNotifyState_weak::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	CheckNull(MeshComp);
	OwnerCharacter = Cast<AGameActor>(MeshComp->GetOwner());
	CheckNull(OwnerCharacter);
	OwnerCharacter->Set_BeParriedDirection(BeParried_Direction);
	OwnerCharacter->Counterable(true);
}

void UCAnimNotifyState_weak::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	CheckNull(MeshComp);
	CheckNull(OwnerCharacter);
	OwnerCharacter->Set_BeParriedDirection(EDirection::Max);
	OwnerCharacter->Counterable(false);
}
