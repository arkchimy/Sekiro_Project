#include "Actors/CArmor.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"


#include "Particles/ParticleSystemComponent.h"

#include "ActorComponents/State_Component/CStateComponent_Armor.h"
#include "ActorComponents/State_Component/CStateComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Sound/SoundCue.h"

ACArmor::ACArmor()
{
	CHelpers::CreateComponent<USceneComponent>(this, &root, "root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &TP_Mesh, "Third_Person", root);
	//CHelpers::CreateComponent<USkeletalMeshComponent>(this, &FP_Mesh, "First_Person", root);
	

	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Attach_Particle, "Attach_Particle", TP_Mesh);
	CHelpers::CreateActorComponent<UCStateComponent_Armor>(this, &Armor_State, "Armor_state");


	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Broken_Particle, "Broken_Particle", TP_Mesh);
	RootComponent = root;
	
	TP_Mesh->SetGenerateOverlapEvents(true);
	//FP_Mesh->SetGenerateOverlapEvents(true);//hit 는 수동 조작

	//FP_Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel6);
	TP_Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel6);

	Broken_Particle->SetVisibility(false);

}

void ACArmor::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);
	state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	player = Cast<ACPlayer>(OwnerCharacter);
	Enemy = Cast<ACEnemy>(OwnerCharacter);

	

	if(!!player)
	{
		//FP_Mesh->bOwnerNoSee = true;
		TP_Mesh->bOnlyOwnerSee = true;

		On_UnEquip();
		
	}
	if(!!Enemy)
	{
		//FP_Mesh->bOnlyOwnerSee = true;
		TP_Mesh->bOwnerNoSee = true;
	
		Enemy->DeadDelegate.AddDynamic(this, &ACArmor::Armor_BrokenMontage);
	}
	
	
}

void ACArmor::On_Equip()
{
	Equip_Armor();
	if (!!player) 
	{
		//FP_Mesh->SetVisibility(true);
		TP_Mesh->SetVisibility(true);
		//FP_Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	TP_Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ACArmor::On_UnEquip()
{
	UnEquip_Armor();
	if (!!player) 
	{
		//FP_Mesh->SetVisibility(false);
		TP_Mesh->SetVisibility(false);
		//FP_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	TP_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}



void ACArmor::Armor_AttachToSocket(FName Socket)
{
	if (!!player)
	{
		TP_Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
		TP_Mesh->AttachToComponent(player->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Socket);
		TP_Mesh->SetCastHiddenShadow(true);
		
	}
	else if(!!Enemy)
	{
		//FP_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TP_Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);

		TP_Mesh->AttachToComponent(Enemy->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative,true), FName("Shield_Socket"));

		//FP_Mesh->SetVisibility(false);
	}
}



void ACArmor::Armor_TakeDamage(float Power, FString Attacker)
{
	Armor_State->Sub_Health(Power, Attacker);
	
}

void ACArmor::Armor_BlockMontage()
{
	
	CheckFalse(Armor_State->IsLife()); // 쉴드가 살아있다면
	CheckTrue(state->IsDeathState());

	if(!!player && state->IsBlockState())
	{
		bool Play_chk = false;
		if (!!Block_Montage.Montage) 
		{
			Play_chk = true;
			player->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f);
			player->GetMesh()->GetAnimInstance()->Montage_Play(Block_Montage.Montage, Block_Montage.PlayRatio);
		}
		if (!!Block_Montage.FP_Montage)
		{
			Play_chk = true;
			player->Get_FP_Mesh()->GetAnimInstance()->Montage_Stop(0.1f);
			player->Get_FP_Mesh()->GetAnimInstance()->Montage_Play(Block_Montage.FP_Montage, Block_Montage.FP_PlayRatio);
		}
		if(Play_chk)
			state->Set_ActionMode();
	}
	
	else if(!!Enemy)
	{
		bool Play_chk = false;
		if (!!Block_Montage.Montage)
		{
			
			state->Set_ActionMode();
			
			Enemy->GetMesh()->GetAnimInstance()->Montage_Stop(0.01f);
			Enemy->GetMesh()->GetAnimInstance()->Montage_Play(Block_Montage.Montage, Block_Montage.PlayRatio);
			Play_chk = true;
		}
		if (Play_chk)
			state->Set_ActionMode();
	}
	
	
}

void ACArmor::Armor_BrokenMontage()
{

	
	if (!!player)
	{
		bool Play_chk = false;
		if (!!Broken_Montage.Montage)
		{
			Play_chk = true;
			player->GetMesh()->GetAnimInstance()->Montage_Stop(0.1f);
			player->GetMesh()->GetAnimInstance()->Montage_Play(Broken_Montage.Montage, Broken_Montage.PlayRatio);
		}
		if (!!Broken_Montage.FP_Montage)
		{
			Play_chk = true;
			player->Get_FP_Mesh()->GetAnimInstance()->Montage_Stop(0.1f);
			player->Get_FP_Mesh()->GetAnimInstance()->Montage_Play(Broken_Montage.FP_Montage, Broken_Montage.FP_PlayRatio);
		}
		if (Play_chk)
			state->Set_ActionMode();
	}

	else if (!!Enemy)
	{
		TP_Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bool Play_chk = false;
		if (!!Broken_Montage.Montage)
		{
			state->Set_ActionMode();

			Enemy->GetMesh()->GetAnimInstance()->Montage_Stop(0.01f);
			Enemy->GetMesh()->GetAnimInstance()->Montage_Play(Broken_Montage.Montage, Broken_Montage.PlayRatio);
			
			
			Play_chk = true;
		}
		if (Play_chk)
			state->Set_ActionMode();
		Broken_Event();
	}
	Enemy->DeadDelegate.RemoveDynamic(this, &ACArmor::Armor_BrokenMontage);
}

void ACArmor::Broken_Event()
{

	if (!!Enemy) 
	{
		CheckFalse(Enemy->GetLife_Chk());
		UParticleSystem* particle = Broken_Particle->Template;
		if (!!particle)
		{
			Broken_Particle->SetVisibility(true);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particle, TP_Mesh->GetRelativeTransform());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Broken_Cue, GetOwner()->GetActorLocation());
		}
	}
}

