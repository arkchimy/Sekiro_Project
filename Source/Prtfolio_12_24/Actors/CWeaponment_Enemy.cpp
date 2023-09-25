#include "Actors/CWeaponment_Enemy.h"
#include "Global.h"

#include "Actors/GameActor.h"


#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "Sound/SoundCue.h"

#include "Utilities/CEnums.h" 


void ACWeaponment_Enemy::On_Collision(EAttackTag_Type AttackType, float Knoback)
{
	GameActor = Cast<AGameActor>(OwnerCharacter);
	for (UShapeComponent* component : ShapeComponent)
	{
		Match_Collision(component);
	}
	AttackTag_Type = AttackType;
	Knoback_Power = Knoback;
}


void ACWeaponment_Enemy::Off_Collision()
{
	Overlap_Array.Empty();

	for (UShapeComponent* component : ShapeComponent)
	{
		component->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
		component->SetGenerateOverlapEvents(false);
		component->SetVisibility(false);
		//component->SetHiddenInGame(true);
	}
	
	Weapon->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4); // Channel3  WeaponDynamic
	
}





void ACWeaponment_Enemy::Match_Collision(UShapeComponent* component)
{
	FName Collision_tag = FName("None");
	TArray<FName> tags;
	tags.Empty();
	for (ECollisionTags tag : collison_tags)
	{
		switch (tag)
		{
		case ECollisionTags::L_Hand:
			tags.AddUnique(FName("L_Hand"));
			continue;
		case ECollisionTags::R_Hand:
			tags.AddUnique(FName("R_Hand"));
			continue;
		case ECollisionTags::Shield:
			tags.AddUnique(FName("Shield"));
			continue;
		case ECollisionTags::Head:
			tags.AddUnique(FName("Head"));

		}
	}
	for (FName tag : tags) 
	{
		if (component->ComponentHasTag(tag))
		{
			component->SetGenerateOverlapEvents(true);
			component->SetHiddenInGame(false);
			//component->SetVisibility(true);
			component->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel9);

		}
	}
	
	
	
}

void ACWeaponment_Enemy::Box_Attach(UShapeComponent* component) //  NoWeapon 을 True 체크했다면 Box collision에 연결시킬 BoneName 을 Tag에 적어둘것.
{
	CheckFalse(NoWeapon);
	FName tag = component->ComponentTags[0];// 첫번째있는 tag 를 가져와서 
	component->AttachToComponent(GameActor->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), tag);
}
