#include "CStatusComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCStatusComponent::UCStatusComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}



void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	hp = Max_Hp;
}


void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UCStatusComponent::Add_Health(float inVal)
{
	hp -= inVal;
}

void UCStatusComponent::Sub_Health(float inVal)
{
	hp -= inVal;
	
}

void UCStatusComponent::Add_Mana(float inVal)
{
	mp -= inVal;
}

void UCStatusComponent::Sub_Mana(float inVal)
{
	mp -= inVal;
}
