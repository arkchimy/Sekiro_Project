#include "Actors/CThrowActors/CThrow_AttachDummy.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"
#include "Particles/ParticleSystem.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

#include "Sound/SoundCue.h"
#include "GameFramework/Character.h"

ACThrow_AttachDummy::ACThrow_AttachDummy()
{
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Throw_DummyMesh");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle_Comp, "Particle_Component",Mesh);

	
}

void ACThrow_AttachDummy::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5);
	Mesh->ComponentTags.Add(FName("Fire"));
	Mesh->SetGenerateOverlapEvents(true);
}


void ACThrow_AttachDummy::Set_StaticMesh(UStaticMesh* mesh)
{
	Mesh->SetStaticMesh(mesh);
}

void ACThrow_AttachDummy::Set_Visible(bool inval)
{
	Mesh->SetVisibility(inval);
	if (!inval) 
	{
		Particle_Comp->SetVisibility(inval);
		State = FName("Normal");
	}
}

void ACThrow_AttachDummy::Set_Owner(ACharacter* InCharacter)
{
	OwnerCharacter = InCharacter;
	if (!!OwnerCharacter) 
	{
		ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
		ACEnemy* enemy = Cast<ACEnemy>(OwnerCharacter);
		if (!!player) 
		{
			AttachToComponent(player->Get_FP_Mesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative,true), "Arrow_Socket");
			
		}
		else if(!!enemy)
		{
			AttachToComponent(enemy->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Arrow_Socket");
		}
	}
}

void ACThrow_AttachDummy::Trigger_TagChk(FName val)
{
	CheckTrue(val == State);
	for(FParticle data : particle)
	{
		if(data.Tag == val)
		{
			State = val;
			Particle_Comp->SetTemplate(data.particle);
			Particle_Comp->SetVisibility(true);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), data.Particle_Sound,RootComponent->GetComponentLocation());
			CLog::Log("Fire On");
			break;
		}
	}
}

