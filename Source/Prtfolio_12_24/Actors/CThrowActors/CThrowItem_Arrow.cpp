#include "Actors/CThrowActors/CThrowItem_Arrow.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"
#include "Actors/CArmor.h"


#include "Camera/CameraComponent.h"
#include "Components/ShapeComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"

#include "Sound/SoundCue.h"

#include "ActorComponents/CActorComponent_Particle.h"

ACThrowItem_Arrow::ACThrowItem_Arrow()
{
	/*Attach_Tag.Add(FName("Enemy"));
	Block_Tag.Add(FName("Steel"));*/
}

void ACThrowItem_Arrow::BeginPlay()
{

	Super::BeginPlay();//Debug
	//Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACThrowItem_Arrow::OnOverlap);
	PrimaryActorTick.bCanEverTick = true;
	
}
void ACThrowItem_Arrow::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);


}


void ACThrowItem_Arrow::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{

	
}


void ACThrowItem_Arrow::OnThrowComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Attachk_chk = true; // 확인


	
	CheckTrue(bBlocked);
	CheckTrue(HitComponent->GetOwner() == OtherActor);
	CheckTrue(HitComponent->GetOwner() == OtherActor->GetOwner());
	CheckTrue(HitComponent->GetOwner() == OtherComponent->GetOwner());
	CheckTrue(OtherActor->GetOwner() == OwnerCharacter);
	CheckTrue(OtherActor == OwnerCharacter);
	
	Power = Orizinal_Power;
	
	//  --- State 가 호환되는 대상과 부딪힐 경우 ex) Attach_Fire 를 hit.actor에 On시킴
	UCActorComponent_Particle* attach_particle = CHelpers::GetComponent<UCActorComponent_Particle>(OtherActor);
	if (!!attach_particle) 
	{
		attach_particle->Attach_Particle(State, 15.0f);
		
	}
	else
	{
		attach_particle = CHelpers::GetComponent<UCActorComponent_Particle>(OtherComponent->GetOwner());
		if (!!attach_particle)
		{
			attach_particle->Attach_Particle(State, 15.0f);

		}
	}
	//                      ----------------------------------------------------- 

	ACharacter* Hit_Character = Cast<ACharacter>(OtherActor);

	FName hit_bone = Hit.BoneName;
	FVector location = Mesh->GetRelativeLocation();

	bool Hit_Collision = false;
	bool Attach_collision = false;
	bool Block_collision = false;
	

	TArray<FName> Tag_list; // 대상 tag 전부 가져오기. 
	Tag_list = OtherComponent->ComponentTags;

	
	for (FName temp : Tag_list) //Tag 비교
	{

		for (FName Hit_tag : Tag_Hit.Tags)
		{
			if (temp == Hit_tag)
			{

				Hit_Collision = true;
				Timer_Reset();
				break;
			}
		}
		if (Hit_Collision)
			break;
	}
	
	for (FName temp : Tag_list) //Tag 비교
	{

		for (FName Attach : Tag_Attach.Tags)
		{
			if (temp == Attach)
			{

				Attach_collision = true;
				break;
			}
		}
		if (Attach_collision)
			break;
	}
	
	

	for (FName temp : Tag_list) //Tag 비교
	{

		for (FName Block : Tag_Block.Tags)
		{
			if (temp == Block)
			{
				
				Block_collision = true;
				bBlocked = true;
				break;
			}
		}
		if (Block_collision)
			break;
	}
	
		
	if(Hit_Collision)
	{
		Attachk_chk = false;
		Mesh->SetVisibility(false);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Tag_Hit.Hit_SoundCue, location);

		FTransform trans;
		trans.SetLocation(Mesh->GetComponentLocation());
		trans.SetRotation(Tag_Hit.ImpactTrans.GetRotation());
		trans.SetScale3D(FVector(Tag_Hit.ImpactTrans.GetScale3D()));

		UParticleSystem* effect = Tag_Hit.Impact_particle;
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect, trans, true);
		UKismetSystemLibrary::K2_ClearTimer(this, "Reset_Visible");
		UKismetSystemLibrary::K2_SetTimer(this, "Reset_Visible", 0.1f, false);
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect, location, GetActorRotation(), true);
	}

	else if (Attach_collision && !bBlocked)
	{
		Attachk_chk = true;
		Attach_Owner = true;// Attach 햇을경우의 bool 변수

		UParticleSystem* effect = Tag_Attach.Blood_particle;
		FRotator rotator = Mesh->GetRelativeRotation();

		if (!!Hit_Character) // 캐릭터라면
		{
		
			Mesh->SetSimulatePhysics(false);
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel11);//Attached_O

			Mesh->AttachToComponent(Hit_Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), hit_bone);
			SetActorRelativeLocation(FVector::ZeroVector);
		
			
			
			
			if (Hit.BoneName.ToString().Contains("head")) // 약점  TArray로 해서 범용성있게 하기
			{
				
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Tag_Attach.Critical_SoundCue, location);
				Power *= 2.0f;
			}
			else
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Tag_Attach.Hit_SoundCue, location);
				// Body shot
			}
			Draw_Route(Hit.Location);

			if (!!effect) 
			{
				FVector hit_location = Hit_Character->GetMesh()->GetSocketLocation(hit_bone);
			

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect, location, GetActorRotation(), true);

				if (ThrowComponentHit.IsBound()) // 실드도 되나?  Attach의 경우 해당 액터에 데미지.
				{
					if (!bBlocked) 
					{
						ThrowComponentHit.Broadcast(Hit, Power, HitComponent->GetOwner());
					}
				}
				
			}

			
		}
		else if(!!OtherComponent) //오브젝트라면
		{
			
			Mesh->SetSimulatePhysics(false);
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			
			Mesh->SetRelativeLocation(FVector::ZeroVector);
			Mesh->SetRelativeRotation(FQuat(Edit.Rot));
			Mesh->AttachToComponent(OtherComponent, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true), hit_bone);
			

			Mesh->SetWorldLocation(Hit.Location);
			Mesh->SetWorldRotation(rotator); //충돌당시 원래의 rotation 으로
			//location += Mesh->GetForwardVector() * 15.0f;
			

			
			Draw_Route(Hit.Location);

			if (!!Tag_Attach.Impact_particle) // 캐릭터가 아니라면 Attach_Effect
			{
				FVector hit_location = Hit.Location;
			
				FTransform trans;
				trans.SetLocation(hit_location);
				trans.SetRotation( Tag_Attach.ImpactTrans.GetRotation());
				trans.SetScale3D(FVector(Tag_Attach.ImpactTrans.GetScale3D()));

				FRotator rot_ = FRotator(trans.GetRotation());
				
				effect = Tag_Attach.Impact_particle;

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect, trans, true);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(),Wood_SoundCue, hit_location);

				ACArmor* armor = Cast<ACArmor>(OtherComponent->GetOwner());

				if (!!armor)
					armor->Armor_TakeDamage(Broken_Power, HitComponent->GetOwner()->GetName());



			}
		}

	}
	if(Block_collision && bBlocked)
	{
		Attachk_chk = false;
	
		
		if(!!Tag_Block.Impact_particle) // 캐릭터가 아니라면 Attach_Effect
		{
			FVector hit_location = Hit.Location;
			FRotator rotator = Mesh->GetRelativeRotation();

			
			FTransform trans;
			trans.SetLocation(location);
			trans.SetRotation(FQuat(rotator) + Tag_Attach.ImpactTrans.GetRotation());
			trans.SetScale3D(FVector(Tag_Block.ImpactTrans.GetScale3D()));
		

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Tag_Block.Impact_particle, trans, true);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Tag_Block.Hit_SoundCue, location);
		
		}
		ACArmor* armor = Cast<ACArmor>(OtherComponent->GetOwner());
		if (!!armor)
			armor->Armor_TakeDamage(Broken_Power, HitComponent->GetOwner()->GetName());
	}
	bBlocked = true;
}


void ACThrowItem_Arrow::Set_Pressed(float inval, ACharacter* InCharacter)
{
	bBlocked = false;
	OwnerCharacter = InCharacter;
	player = Cast<ACPlayer>(InCharacter);

	Mesh->SetSimulatePhysics(true);
	Mesh->SetRelativeLocation(FVector::ZeroVector);
	Mesh->SetRelativeRotation(FQuat(Edit.Rot));

	Set_Throwing(true);

	inval = inval > Max_Inval ? Max_Inval : inval;
	if (!!player)
	{
		UCameraComponent* Camera = player->GetCamera();

		FRotator camera_rot = FRotator(Camera->GetComponentToWorld().GetRotation());
		FRotator val = FRotator(camera_rot.Pitch, camera_rot.Yaw, camera_rot.Roll);  //축이 다른가?
		if(bRYP)
			val = FRotator( camera_rot.Roll, camera_rot.Yaw, camera_rot.Pitch);  //축이 다른가?
		RootComponent->SetWorldRotation(val+ Edit.Rot);
		
		Impulse_Result = player->GetCamera()->GetForwardVector() * Impulse_Power * inval;

		FVector Loc = player->GetActorLocation() + Camera->GetRelativeLocation();
		RootComponent->SetWorldLocation(Loc,false,nullptr,ETeleportType::TeleportPhysics);
		
		Set_Visible(true);
		//Trail_Visible(true);
		
		Mesh->AddImpulseAtLocation(Impulse_Result, Mesh->GetRelativeLocation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Preesed_SoundCue, RootComponent->GetComponentLocation());
		Shoot(Impulse_Result); //FVector값만 보냄
		
	}
	else // 몬스터라면.
	{
		RootComponent->SetWorldRotation(InCharacter->GetActorRotation() + Edit.Rot);
		Impulse_Result = Mesh->GetForwardVector() * Impulse_Power * inval;
		Mesh->SetRelativeLocation(InCharacter->GetMesh()->GetSocketLocation("Arrow_Socket"));

		Set_Visible(true);
		Mesh->SetPhysicsLinearVelocity(Impulse_Result);
	}
	
	UKismetSystemLibrary::K2_SetTimer(this, "Reset_Visible", 6.0f, false);
	UKismetSystemLibrary::K2_SetTimer(this, "Trail_AutoOff", 0.8f, false);
	UKismetSystemLibrary::K2_SetTimer(this, "hit_Posible", 0.003f, false);
}

void ACThrowItem_Arrow::hit_Posible()
{
	//hit = false;
	//Mesh->SetGenerateOverlapEvents(true);
	
}

void ACThrowItem_Arrow::Trail_AutoOff()
{
	Trail_Visible(false);
}


void ACThrowItem_Arrow::Draw_Route(FVector Hit_location)
{ //Draw_Debug
	CLog::Log(Hit_location);
	FVector zero = FVector::ZeroVector;

	FPredictProjectilePathParams params(5.0f, Hit_location, zero,0.1f);
	params.DrawDebugTime = 2.0f;
	
	params.DrawDebugType = EDrawDebugTrace::ForDuration;
	params.OverrideGravityZ = 0.0f;
	FPredictProjectilePathResult result;
	UGameplayStatics::PredictProjectilePath(this, params, result);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Tag_Hit.Hit_SoundCue, Mesh->GetRelativeLocation());
	
}
