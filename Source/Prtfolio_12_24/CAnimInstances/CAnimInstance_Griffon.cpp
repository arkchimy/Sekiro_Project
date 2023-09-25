#include "CAnimInstance_Griffon.h"
#include "Global.h"

#include "Actors/GameActor.h"

#include "GameFramework/Character.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"


void UCAnimInstance_Griffon::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerCharacter = Cast<AGameActor>(GetOwningActor());
	Combat_Component = CHelpers::GetComponent<UCombatComponent>(OwnerCharacter);
	State_Component = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	

}

void UCAnimInstance_Griffon::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	if (!!OwnerCharacter) // Onwer Null 상황시 Catch 과정
	{
		Speed = OwnerCharacter->GetVelocity().Size2D();
		Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());
		Combat_Component = CHelpers::GetComponent<UCombatComponent>(OwnerCharacter);
		Combat = Combat_Component->Get_CurrentCombat();
		State_Component = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
		State = State_Component->Get_State();
	}
	else
	{
		OwnerCharacter = Cast<AGameActor>(GetOwningActor());
	}



}



