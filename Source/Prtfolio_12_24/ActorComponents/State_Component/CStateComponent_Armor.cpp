#include "ActorComponents/State_Component/CStateComponent_Armor.h"
#include "Global.h"
#include "Actors/CArmor.h"

UCStateComponent_Armor::UCStateComponent_Armor()
{
	

}


void UCStateComponent_Armor::BeginPlay()
{
	Armor = Cast<ACArmor>(GetOwner());
	Health = Health_Max;
	Super::BeginPlay();

}

void UCStateComponent_Armor::Sub_Health(float inval,FString Attacker)
{
	CheckFalse(Life);
	if(Armor_Power < inval)
		Health -= inval - Armor_Power;
	if (Health <= 0) 
	{
		
		Armor->Armor_Broken();
		Armor->Armor_BrokenMontage();
		Life = false;
	}
	else
		Armor->Armor_BlockMontage();
}

void UCStateComponent_Armor::Add_Health(float inval)
{
	Health += inval;
}



