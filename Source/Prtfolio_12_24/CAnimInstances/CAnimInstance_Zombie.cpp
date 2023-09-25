#include "CAnimInstance_Zombie.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "GameFramework/Character.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"
#include "ActorComponents/State_Component/CStateComponent.h"

void UCAnimInstance_Zombie::NativeBeginPlay()
{

	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	Combat_Component = CHelpers::GetComponent<UCombatComponent>(OwnerCharacter);

	Super::NativeBeginPlay();

}

void UCAnimInstance_Zombie::NativeUpdateAnimation(float DeltaSeconds)
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
		Life = State_Component->IsDeathState();
		
	}
	else
	{
		OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	}
	


}


