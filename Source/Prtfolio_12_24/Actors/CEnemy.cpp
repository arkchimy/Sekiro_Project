#include "Actors/CEnemy.h"
#include "Global.h"
#include "Sound/SoundCue.h"


#include "Actors/Enemies/CAIController_Enemy.h"
#include "Actors/CThrowActors/CThrow_ManagerActor.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Cameras/CCamera_Manager.h"

#include "Particles/ParticleSystemComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ActorComponent.h" //  Add_Comonenttag

#include "Actors/CPlayer.h"
#include "Actors/CWeaponment.h"
#include "Actors/CAttackments/CAttackment.h"
#include "Actors/CAttackments/CEnemy_Melee.h"

#include "Actors/CArmor.h"
#include "Actors/CThrowActors/CThrow_AttachDummy.h"
#include "Actors/CThrowActors/CThrowItem.h"
#include "Actors/Enemies/CAIController_Enemy.h"

#include "ActorComponents/CStatusComponent.h"
#include "ActorComponents/State_Component/CStateComponent.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"



ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FQuat(FRotator(0, -90, 0)));
	
	Combat_component->ChangeType(ECombat_Type::Unarmed);
	GetMesh()->ComponentTags.AddUnique(FName("Enemy"));
	
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//controller = GetController();
	controller = Cast<ACAIController_Enemy>(GetController());


	GetCharacterMovement()->MaxWalkSpeed = Status->Get_WalkSpeed();
	
	FTransform trans;
	trans.SetLocation(FVector(0, 0, 0));
	if (!!CameraManager_Class)
	{
		Camara_Manager = GetWorld()->SpawnActor<ACCamera_Manager>(CameraManager_Class, trans);
	}

	if (!!ThrowDummy_class)
	{
		Throw_Dummy = GetWorld()->SpawnActor<ACThrow_AttachDummy>(ThrowDummy_class);
		Throw_Dummy->Set_Owner(this);
		
	
	}
	
	trans.SetLocation(FVector(0, 0, 0));
	Throw_Manager = GetWorld()->SpawnActor<ACThrow_ManagerActor>(ThrowManager_class, trans);


	if (!!CameraManager_Class)
	{
		Camara_Manager = GetWorld()->SpawnActor<ACCamera_Manager>(CameraManager_Class, trans);
	}

	State->Set_IdleMode();

	OnFist();
	
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!!controller)
		controller->MoveUpdate(State->CanMove());
}


void ACEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	if (State->IsDeathState())
		return 0;
	if (!!effect.Effect)
	{
		FTransform trans;
		trans.SetScale3D(effect.EffectTransform.GetScale3D());
		trans.SetLocation(effect.EffectTransform.GetLocation() + GetActorLocation());

		
		FRotator rot = hit_roation + FRotator(effect.EffectTransform.GetRotation());
		trans.SetRotation(FQuat(rot));
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect.Effect, trans, true);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), effect.HitSound, trans.GetLocation()); //  Weapon Slash 소리
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), hit_Sound, trans.GetLocation()); // Character 비명
		
	}
	
	Status->Sub_Health(Damage);

	ACThrowItem* throw_item = Cast<ACThrowItem>(DamageCauser);
	
	if (!!throw_item) 
	{
		player = Cast<ACPlayer>(throw_item->Get_OwnerCharacter());
		Last_AttackTag = EAttackTag_Type::Normal;
		Last_Knockback = 50.f;
		
	}
	else
		player = Cast<ACPlayer>(DamageCauser->GetOwner());
	if (!!controller)
		controller->Set_FocusActor(player);
	State->Hitted_PlayMontage(DamageCauser, Last_AttackTag);
	
	if(!!throw_item)
	{
		if (!!player)
			player->Hit_CrossHair();
	}
	if(!!player) //플레이어에게 맞았다면 
	{
		controller = Cast<ACAIController_Enemy>(GetController());
		if (!!controller)
		{
			C_ID = 1;
			controller->Set_TeamId(C_ID);
			controller->Damaged_Target(player); // 때린 대상을 타겟으로 지목
			
		}
	}
	if((Status->GetHp() <= 0) && Life_Chk)
	{
		
		Life_Chk = false;
		//State->Set_DeathMode(); // Nocollision 으로 바꿈.
		
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel8);

		DeathMode();
		if (!!player) 
		{
			player->Kill_CrossHair();

		}
		
		if(Broken_Possible)
		{
			Broken_Mesh(); // Broken Mesh  create
		}
		if (DeadDelegate.IsBound())
		{
			if (!!controller)
				controller->Monster_Dead();
			DeadDelegate.Broadcast();
		}
		
	}
	if (!!controller)
		controller->MoveUpdate(State->CanMove());
	if (!!player)
		cine_player = player;
	return 0.0f;
}

void ACEnemy::Set_HitBone(FName inval)
{
	State->Set_HitBone(inval);
}

void ACEnemy::AttachToCollision(AActor* Weapon, FName SocketName)
{
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SocketName);
}

void ACEnemy::OnFaceMontage()
{
	CheckFalse(rand() % 3 == 0);
	State->Play_FacedMontage();
}

void ACEnemy::Notify_TakeDamage(float Damage)
{
	Status->Sub_Health(Damage);
	CheckTrue(Status->GetHp() > 0);
	CheckNull(player);
	player->Hit_CrossHair();
}

void ACEnemy::DeathMode()
{
	State->Set_DeathMode();
	CheckFalse(bHasCine); // has not cine;
	if(!!cine_player )
	{
		cine_player->DeathMode_Cine_Play();
	}
}

void ACEnemy::Throw_Action_Notify()
{
	Combat_component->L_AttackRelease();
}

void ACEnemy::Resiste_PlayMontage()
{
	Combat_component->Resiste_PlayMontage();
}


void ACEnemy::Weapon_Change(ECombat_Type type)
{
	if (State->IsIdleState())
	{
		if (Combat_component->Get_CurrentCombat() == ECombat_Type::Unarmed)
		{

			Combat_component->ChangeType(type);
			Combat_component->Equip();
		}
		else if (Combat_component->Get_CurrentCombat() == type)
		{

			Combat_component->UnEquip();
			Combat_component->ChangeType(ECombat_Type::Unarmed);
		}
		else
		{
			Combat_component->Set_SwapCombat(type);
			Combat_component->UnEquip();//현재 무기 해제.

		}

	}
}
void ACEnemy::OnFist() { Weapon_Change(ECombat_Type::Fist); }
void ACEnemy::OnSS() { Weapon_Change(ECombat_Type::SS); }
void ACEnemy::OnArcher() { Weapon_Change(ECombat_Type::Archer); }
void ACEnemy::OnWizard() { Weapon_Change(ECombat_Type::Wizard); }


void ACEnemy::Monster_Attack()
{
	CheckFalse(State->IsIdleState());
	FString temp = Combat_component->GetName();
	
	Combat_component->Monster_Attack();
	if(!!controller)
		controller->Random_Select_Operation();
}

UAnimMontage* ACEnemy::Get_NextMontage()
{
	if (State->IsIdleState()) 
	{
		ACAttackment* attackment = Combat_component->Get_ACAttackment();
		if (!!attackment)
		{
			return attackment->Get_NextMontage();
		}
	}
	return nullptr;
}

//void ACEnemy::OnFist()
//{
//	if (State->IsIdleState())
//	{
//		
//		if (Combat_component->Get_CurrentCombat() == ECombat_Type::Unarmed)
//		{
//			CLog::Log("OnFist_Call");
//			Combat_component->ChangeType(ECombat_Type::Fist);
//			Combat_component->Equip();
//		}
//		else if (Combat_component->Get_CurrentCombat() == ECombat_Type::Fist)
//		{
//			CLog::Log("UnEquip_Call");
//			Combat_component->UnEquip();
//			Combat_component->ChangeType(ECombat_Type::Unarmed);
//		}
//		else
//		{
//			CLog::Log("Swap_Call");
//			Combat_component->Set_SwapCombat(ECombat_Type::Fist);
//			Combat_component->UnEquip();//현재 무기 해제.
//
//		}
//		
//	}
//
//
//}
//
//void ACEnemy::OnSS()
//{
//	if (State->IsIdleState())
//	{
//		//State->Set_EquipMode();
//		if (Combat_component->Get_CurrentCombat() == ECombat_Type::Unarmed)
//		{
//			CLog::Log("OnSS_Call");
//			Combat_component->ChangeType(ECombat_Type::SS);
//			Combat_component->Equip();
//		}
//		else if (Combat_component->Get_CurrentCombat() == ECombat_Type::SS)
//		{
//			CLog::Log("UnEquip_Call");
//			Combat_component->UnEquip();
//		}
//		else
//		{
//			CLog::Log("Swap_Call");
//			Combat_component->UnEquip();//현재 무기 해제.
//			Combat_component->Set_SwapCombat(ECombat_Type::SS);
//
//		}
//
//	}
//
//}
//
//void ACEnemy::OnArcher()
//{
//	if (State->IsIdleState())
//	{
//		if (Combat_component->Get_CurrentCombat() == ECombat_Type::Unarmed)
//		{
//			CLog::Log("OnArcher_Call");
//			Combat_component->ChangeType(ECombat_Type::Archer);
//			Combat_component->Equip();
//		}
//		else if (Combat_component->Get_CurrentCombat() == ECombat_Type::Archer)
//		{
//			CLog::Log("UnEquip_Call");
//			Combat_component->UnEquip();
//
//		}
//		else
//		{
//			CLog::Log("Swap_Call");
//			Combat_component->UnEquip();//현재 무기 해제.
//			Combat_component->Set_SwapCombat(ECombat_Type::Archer);
//
//		}
//
//	}
//}
//
//void ACEnemy::OnWizard()
//{
//	if (State->IsIdleState())
//	{
//		if (Combat_component->Get_CurrentCombat() == ECombat_Type::Unarmed)
//		{
//			CLog::Log("OnWizard_Call");
//			Combat_component->ChangeType(ECombat_Type::Wizard);
//			Combat_component->Equip();
//		}
//		else if (Combat_component->Get_CurrentCombat() == ECombat_Type::Wizard)
//		{
//			CLog::Log("UnEquip_Call");
//			Combat_component->UnEquip();
//		}
//		else
//		{
//			CLog::Log("Swap_Call");
//			Combat_component->UnEquip();//현재 무기 해제.
//			Combat_component->Set_SwapCombat(ECombat_Type::Wizard);
//
//		}
//
//	}
//}