#include "CCombatData.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "DataTable/FHitData.h"

#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"
#include "Actors/CWeaponment.h"
#include "Actors/CEquipment.h"
#include "Actors/CAttackments/CAttackment.h"

void UCCombatData::CombatData_Init(const UCCombatData& other)
{
	//Deep Copy
	if (this == &other)
		return ;

	EquipmentClass = other.EquipmentClass;
	WeaponmentClass = other.WeaponmentClass;
	AttackTypeClass = other.AttackTypeClass;
	ParryingTable = other.ParryingTable;
	BeParried_Table = other.BeParried_Table;

	CrossHair = other.CrossHair;
	Weapon_ComboTable = other.Weapon_ComboTable;
	Sight = other.Sight;
	Action_Table = other.Action_Table;

	

}

void UCCombatData::BeginPlay(class ACharacter* InOwnerCharacter)
{
	FTransform transform;
	OwnerCharacter = InOwnerCharacter;
	transform.SetLocation(OwnerCharacter->GetActorLocation());
	if (!!WeaponmentClass)
	{
		
		Weaponment = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACWeaponment>(WeaponmentClass, transform, OwnerCharacter);
		UGameplayStatics::FinishSpawningActor(Weaponment, transform);

		Weaponment->SetActorLabel(GetLableName(OwnerCharacter, Weaponment->GetName()));
		Weaponment->AttachTo(Weaponment->Holster);
		Weaponment->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

		if (!!EquipmentClass)
		{
			
			Equipment = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>(EquipmentClass, transform, OwnerCharacter);
			UGameplayStatics::FinishSpawningActor(Equipment, transform);
			Equipment->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));	
			Equipment->SetActorLabel(GetLableName(OwnerCharacter, "Equipment"));
			
			if (!!Weaponment)
			{
				Equipment->OnEquipmentDelegate.AddDynamic(Weaponment, &ACWeaponment::AttachTo);
				Equipment->OnUnequipmentDelegate.AddDynamic(Weaponment, &ACWeaponment::AttachTo);
			}
		}
		if (!!AttackTypeClass)
		{
			Attackment = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttackment>(AttackTypeClass, transform, OwnerCharacter);
			UGameplayStatics::FinishSpawningActor(Attackment, transform);

			//Attackment->AttachToActor(OwnerCharacter, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
			//Attackment->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
			Attackment->SetActorLabel(GetLableName(OwnerCharacter, "__Attackment"));

			if (!!Weapon_ComboTable) // ComboTable Attackment ¿¡ SetÇÏ±â
				Attackment->Set_ComboTable(Weapon_ComboTable);

			Attackment->Set_ThrowItemID(Wheel_num);
			Attackment->Set_SkillThrowID(Skill_Throw);

			if (!!Weaponment)
			{
				Weaponment->TakeDamage_Delegate.AddDynamic(Attackment, &ACAttackment::TakeOverlap_Actor);
				Attackment->WeaponNiagara_Delegate.AddDynamic(Weaponment, &ACWeaponment::Weapon_Niagara);
				ACEnemy* enemy = Cast<ACEnemy>(OwnerCharacter);
				if (!!enemy)
					Attackment->AttackTag_Delegate.AddDynamic(Weaponment, &ACWeaponment::Set_CollisionTag);
			}


		}
	}

	
	if(!!Equipment)
	{
		Equipment->Cross_Hair.AddDynamic(this, &UCCombatData::SendCrossHair);
	}

	Weaponment->BeginPlay(InOwnerCharacter);

	CheckNull(ParryingTable);
	if (!!ParryingTable)
	{
		Parry_Data_size = ParryingTable->GetRowNames();
		CLog::Log(GetName() + "ParryTable Init");
	}
	else
		CLog::Print("ParryingMontage is Null");

	CheckNull(BeParried_Table);
	if (!!BeParried_Table)
	{
		BeParryData_size = BeParried_Table->GetRowNames();
		CLog::Log(GetName() + "BeParried_Table Init");
	}
	else
		CLog::Print("BeParried_Table is Null");
}

void UCCombatData::DestroyActor()
{
	Weaponment->Armor_Destroy();
	Weaponment->Destroy();
	Equipment->Destroy();
	Attackment->Destroy();
	
}

ACEquipment* UCCombatData::Get_Equipment()
{
	if (!!Equipment) 
	{
		return Equipment;
	}

	return nullptr;
	

}



void UCCombatData::SendCrossHair()
{
	
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (!!player)
	{
		player->SetCrossHair(CrossHair);

	}
}

FString UCCombatData::GetLableName(ACharacter* inOwner, FString name)
{
	FString temp;
	temp = inOwner->GetName() + name;
	return temp;
}



void UCCombatData::ParryingMontage(EDirection direction)
{
	
	for (int i = 0; i < Parry_Data_size.Num(); i++)
	{
		FParrying_Data temp = *(ParryingTable->FindRow<FParrying_Data>(Parry_Data_size[i], Parry_Data_size[i].ToString()));
		if (temp.direction == direction) 
		{
			OwnerCharacter->PlayAnimMontage(temp.Montage, temp.PlayRatio, temp.StartSection);
			return;
		}
	}
}

void UCCombatData::BeParriedMontage(EDirection direction,ACharacter* ActionActor)
{
	for (int i = 0; i < BeParryData_size.Num(); i++)
	{
		FParrying_Data temp = *(BeParried_Table->FindRow<FParrying_Data>(BeParryData_size[i], BeParryData_size[i].ToString()));
		if (temp.direction == direction)
		{
			
			ActionActor->PlayAnimMontage(temp.Montage, temp.PlayRatio, temp.StartSection);
			return;
		}
	}
	//CLog::Print(BeParryData_size.Num());
	
}



void UCCombatData::Slot_Swap(ACWeaponment* SwapWeapon)
{
	ACWeaponment* temp = Weaponment;
	Weaponment = SwapWeapon;
	
	temp->Destroy_Armor();
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (!!player)
		player->Remove_ShowActor(temp);
	temp->Destroy();

	if (!!Weaponment)
	{
		/*Equipment->OnEquipmentDelegate.AddDynamic(Weaponment, &ACWeaponment::AttachTo);
		Equipment->OnUnequipmentDelegate.AddDynamic(Weaponment, &ACWeaponment::AttachTo);*/
		CLog::Log("Attach_Swap Equip");
		CLog::Log(Weaponment->GetName());
	}

	if (!!Weaponment)
	{
		Weaponment->TakeDamage_Delegate.AddDynamic(Attackment, &ACAttackment::TakeOverlap_Actor);
		ACEnemy* enemy = Cast<ACEnemy>(OwnerCharacter);
		if (!!enemy)
			Attackment->AttackTag_Delegate.AddDynamic(Weaponment, &ACWeaponment::Set_CollisionTag);
	}

}
