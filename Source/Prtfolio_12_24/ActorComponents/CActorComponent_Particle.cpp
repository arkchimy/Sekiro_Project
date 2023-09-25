#include "CActorComponent_Particle.h"
#include "Global.h"
#include "Actors/CArmor.h"
#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"

#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"

UCActorComponent_Particle::UCActorComponent_Particle()
{
	PrimaryComponentTick.bCanEverTick = true;
}



void UCActorComponent_Particle::BeginPlay()
{
	Super::BeginPlay();
	armor = Cast<ACArmor>(GetOwner());
	player = Cast<ACPlayer>(GetOwner());
	enemy = Cast<ACEnemy>(GetOwner());
	if (!!armor)
		comp = armor->Get_AttachParticle();
	else if (!!player)
		comp = player->Get_AttachParticle();
	else if (!!enemy) 
	{
		comp = enemy->Get_AttachParticle();
		
	}

	

}


void UCActorComponent_Particle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCActorComponent_Particle::Attach_Particle(FName tag,float timer)
{

	for(FParticle data : possible_Particle)
	{
		if (data.Tag == tag)
		{
			comp->SetTemplate(data.particle);
			comp->SetVisibility(true);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), data.Particle_Sound, GetOwner()->GetActorLocation());
			UKismetSystemLibrary::K2_SetTimer(this, "Auto_ParticleOff", timer, false);

			if (data.Tag == FName("Fire")) 
			{
				Attached_Damaged();
				UKismetSystemLibrary::K2_SetTimer(this, "Attached_Damaged", 1.5f, true);
			}
			break;
		}
	}
	
}

void UCActorComponent_Particle::Auto_ParticleOff()
{
	comp->SetVisibility(false);
	UKismetSystemLibrary::K2_ClearTimer(this, "Attached_Damaged");
}

void UCActorComponent_Particle::Attached_Damaged()
{
	armor = Cast<ACArmor>(GetOwner());
	player = Cast<ACPlayer>(GetOwner());
	enemy = Cast<ACEnemy>(GetOwner());
	FDamageEvent Event_Type;
	if (!!armor) 
	{
		armor->Armor_TakeDamage(Fire_Damage, "Fire");
		
	}
	else if (!!enemy) 
	{
		AController* controll = enemy->GetController();
		enemy->TakeDamage(Fire_Damage, Event_Type, controll, GetOwner());
	}
}

