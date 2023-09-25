#include "CAttackment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"
#include "Actors/GameActor.h"

#include "ActorComponents/State_Component/CStateComponent.h"
#include "ActorComponents/CStatusComponent.h"


ACAttackment::ACAttackment()
{
	
}

void ACAttackment::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	OwnerActor = Cast<AGameActor>(OwnerCharacter);
	player = Cast<ACPlayer>(OwnerCharacter);
	enemy = Cast<ACEnemy>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	
	
}


void ACAttackment::Set_ComboTable(UDataTable* table)
{
	ComboTable = table;
	ComboRow = ComboTable->GetRowNames(); // DataTable의 Size와 몇 번째  Montage인지 확인할 용도
	
}

