#include "Actors/CThrowActors/CThrowItem.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "Components/ShapeComponent.h"
#include "Actors/CThrowActors/CThrow_ManagerActor.h"


#include "Actors/CAttackments/CAttackment_Archer.h"
#include "Particles/ParticleSystemComponent.h"

#include "Sound/SoundCue.h"

#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACThrowItem::ACThrowItem()
{
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "mesh");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle_Comp, "Particle_Comp", Mesh);

	RootComponent = Mesh;
	
	Mesh->SetVisibility(false);
	Mesh->SetRelativeRotation(FQuat(Edit.Rot));
	Mesh->SetRelativeScale3D(Edit.Scale);
}
void ACThrowItem::BeginPlay()
{

	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
	Mesh->OnComponentHit.AddDynamic(this, &ACThrowItem::OnThrowComponentHit);

	Orizinal_Power = Power;


}
void ACThrowItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACThrowItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	
	
}

void ACThrowItem::OnThrowComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	CheckTrue(bBlocked);
	CheckTrue(HitComponent->GetOwner() == OtherActor);
	CheckTrue(HitComponent->GetOwner() == OtherComponent->GetOwner());
	
	if (Hit.Component->ComponentHasTag("Enemy"))
	{

		AActor* HitActor = Cast<AActor>(Hit.Actor.Get());

		ACharacter* HitEnemy = Cast<ACharacter>(Hit.Actor.Get());
		ACharacter* Hit_Character = Cast<ACharacter>(OtherActor);
		FName hit_bone = Hit.BoneName;

		if (!!HitEnemy) // 약점 부위 타격
		{
			if (Hit.BoneName.ToString().Contains("head"))
			{
				CLog::Print("Head shot");
				Power *= 1.2f; //  크리티컬 데미지.
			}
		}
		UParticleSystem* effect = Tag_Attach.Impact_particle;

		if (!!effect && !!Hit_Character)
		{
			FVector hit_location = Hit_Character->GetMesh()->GetSocketLocation(hit_bone);
			FTransform transform;
			transform.SetLocation(hit_location);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect, hit_location, GetActorRotation(), true);

		}

		if (ThrowComponentHit.IsBound())
		{
			ThrowComponentHit.Broadcast(Hit, Power, HitComponent->GetOwner());
		}
		Power = Orizinal_Power;
	}
	bBlocked = true;
}

void ACThrowItem::Set_Visible(bool inval)
{
	Mesh->SetVisibility(inval);
}
bool ACThrowItem::Get_Throwing()
{
	return Throwing; 
}
bool ACThrowItem::Get_Visible()
{
	return Mesh->GetVisibleFlag();
}


void ACThrowItem::Reset_Visible()
{
	
	Particle_Comp->SetVisibility(false);
	State = FName("None");
	Attach_Owner = false;

	
	Trail_Visible(false);
	AttachToActor(Manager, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5);
	Set_Visible(false);
	Set_Throwing(false);

	Mesh->SetRelativeScale3D(Orizinal_Scale);
	
}

void ACThrowItem::Timer_Boom()
{
	FVector location = Mesh->GetRelativeLocation();

	UParticleSystem* effect = Tag_Hit.Impact_particle;

	FTransform trans;
	trans.SetLocation(Mesh->GetComponentLocation());
	trans.SetRotation(Tag_Hit.ImpactTrans.GetRotation());
	trans.SetScale3D(FVector(Tag_Hit.ImpactTrans.GetScale3D()));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect, trans, true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Tag_Hit.Hit_SoundCue, Mesh->GetRelativeLocation());

}

void ACThrowItem::Set_Manager(ACThrow_ManagerActor* owner)
{
	Manager = owner; 
	AttachToActor(Manager, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	Trail_Visible(false);
	Orizinal_Scale = Mesh->GetRelativeScale3D();
}

void ACThrowItem::Set_State(FName val)
{
	if (val == FName("Normal"))
		Particle_Comp->SetVisibility(false);
	else
		for(FParticle data : Particle)
		{
			if(data.Tag == val)
			{
				Particle_Comp->SetTemplate(data.particle);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), data.Particle_Sound, RootComponent->GetComponentLocation());
				//Particle_Comp->SetRelativeLocation(FVector(0, 0, 0));
				State = data.Tag;
				Particle_Comp->SetVisibility(true);
			}
		}
	
	
}

void ACThrowItem::Set_Pressed(float inval, ACharacter* InCharacter)
{
	
	Create_Location = GetActorLocation();
	Set_Visible(true);
	OwnerCharacter = InCharacter;
	ACPlayer* player = Cast<ACPlayer>(InCharacter);
	if (!!player)
	{
		Mesh->SetRelativeLocation(player->Get_FP_Mesh()->GetSocketLocation("ArrowSocket"));
		
	}
	else
	{
		Mesh->SetRelativeLocation(player->Get_FP_Mesh()->GetSocketLocation("ArrowSocket"));
		
	}
	
	bBlocked = false;
	
	UKismetSystemLibrary::K2_SetTimer(this, "Reset_Visible", 5.0f, false);
}
