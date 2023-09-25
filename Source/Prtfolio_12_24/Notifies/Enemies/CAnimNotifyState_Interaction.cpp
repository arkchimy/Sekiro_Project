#include "CAnimNotifyState_Interaction.h"
#include "Global.h"
#include "Actors/CEnemy.h"

FString UCAnimNotifyState_Interaction::GetNotifyName()const
{
	return "Interaction";
}

void UCAnimNotifyState_Interaction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	enemy = Cast<ACEnemy>(MeshComp->GetOwner());

	if(!!enemy)
	{
		enemy->Set_Interactive(true);
	}
}

void UCAnimNotifyState_Interaction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	enemy = Cast<ACEnemy>(MeshComp->GetOwner());

	if (!!enemy)
	{
		enemy->Set_Interactive(false);
	}
}
