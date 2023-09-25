#include "CPageCombatComponent.h"
#include "Global.h"

#include "Actors/Builes/CPageMaster.h"

UCPageCombatComponent::UCPageCombatComponent()
{

}


void UCPageCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCPageCombatComponent::Set_PickupMode()
{
	combat = EPageCombat::PickUp;
	Page->Get_Mesh()->SetVisibility(true);
}

void UCPageCombatComponent::Set_S_BackMode()
{
	combat = EPageCombat::S_Back;
	Page->Get_Mesh()->SetVisibility(true);
}

void UCPageCombatComponent::Set_S_FrontMode()
{
	combat = EPageCombat::S_Front;
	Page->Get_Mesh()->SetVisibility(true);
}

void UCPageCombatComponent::Set_BackMode()
{
	combat = EPageCombat::Back;
	Page->Get_Mesh()->SetVisibility(true);
}

void UCPageCombatComponent::Set_FrontMode()
{
	combat = EPageCombat::Front;
	Page->Get_Mesh()->SetVisibility(true);
}

void UCPageCombatComponent::Set_InvisibleMode()
{
	combat = EPageCombat::invisible;
	Page->Get_Mesh()->SetVisibility(false);
}

