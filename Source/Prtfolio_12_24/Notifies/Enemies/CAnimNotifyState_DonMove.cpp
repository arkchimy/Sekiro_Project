#include "Notifies/Enemies/CAnimNotifyState_DonMove.h"
#include "Global.h"
#include "Actors/CEnemy.h"
#include "Actors/Enemies/CAIController_Enemy.h"


FString UCAnimNotifyState_DonMove::GetNotifyName()const
{
	return "BB_DonMove";
}

void UCAnimNotifyState_DonMove::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	enemy = Cast<ACEnemy>(MeshComp->GetOwner());
	
	if (!!enemy) 
		controller = Cast<ACAIController_Enemy>(enemy->GetController());
	if(!!controller)
	{
		controller->MoveUpdate(false);
	}
}

void UCAnimNotifyState_DonMove::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!!controller)
	{
		controller->MoveUpdate(true);
	}
}
