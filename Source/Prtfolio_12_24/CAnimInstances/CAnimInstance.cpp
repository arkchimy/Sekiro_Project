#include "CAnimInstance.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ActorComponents/Combat_Component/CombatComponent.h"



void UCAnimInstance::NativeBeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwningActor());

	Super::NativeBeginPlay();
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	Init_Animinstance();
	CheckNull(OwnerCharacter);
	CheckNull(Combat_Component);

	if (!!OwnerCharacter) // Onwer Null 상황시 Catch 과정
	{
		Speed = OwnerCharacter->GetVelocity().Size2D();
		Speed = OwnerCharacter->GetMovementComponent()->Velocity.Size2D();
		
		Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());
		Combat = Combat_Component->Get_CurrentCombat();

	}
	if (!!FeetComp)
		FeetData = FeetComp->Get_FeetData();
}

void UCAnimInstance::Init_Animinstance()
{
	if (!!OwnerCharacter) { return ; }
	else
	{
		OwnerCharacter = Cast<ACharacter>(GetOwningActor());
		if (!!OwnerCharacter)
		{
			Combat_Component = CHelpers::GetComponent<UCombatComponent>(OwnerCharacter);
			FeetComp = CHelpers::GetComponent<UCActorComponent_IKFeet>(OwnerCharacter);
		}

	}
}

//void UCAnimInstance::BlueprintBeginPlay()
//{
//
//	CLog::Log("AnimNotifyBluePrintBegin_");
//
//	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
//	
//	CLog::Log(OwnerCharacter);
//
//	Super::NativeBeginPlay();
//}
//