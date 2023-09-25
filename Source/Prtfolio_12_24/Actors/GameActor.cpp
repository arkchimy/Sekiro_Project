#include "Actors/GameActor.h"
#include "Global.h"

#include "GameFramework/SpringArmComponent.h"


#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h" // Forward
#include "Components/BoxComponent.h"

#include "ActorComponents/Combat_Component/CombatComponent.h"
#include "ActorComponents/CStatusComponent.h"
#include "ActorComponents/State_Component/CStateComponent.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"
#include "ActorComponents/CActorComponent_Particle.h"

#include "Particles/ParticleSystemComponent.h"

#include "Camera/CameraComponent.h"

AGameActor::AGameActor()
{
 
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "StatusComponent");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "StateComponent");
	CHelpers::CreateActorComponent<UCombatComponent>(this, &Combat_component, "Combat_Component");
	CHelpers::CreateActorComponent<UCActorComponent_Particle>(this, &Particle_ActorComp, "Particle_ActorComp");
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Attach_Particle, "Attach_Particle", GetMesh());

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm_Component", GetCapsuleComponent());
	CHelpers::CreateComponent<UBoxComponent>(this, &ParryingBoxComp, "ParryingBox", GetCapsuleComponent());

	
	SpringArm->SetRelativeLocation(FVector(0, 0, 0));
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;

	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera_Component", SpringArm);
	Camera->SetRelativeLocation(FVector(0, 0, 60));
	Camera->SetAutoActivate(true);

	
}

void AGameActor::BeginPlay()
{
	Super::BeginPlay();
	
	GetMesh()->SetAnimInstanceClass(Mesh_animInstance);
	ParryingBoxComp->SetGenerateOverlapEvents(false);
}

void AGameActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AGameActor::GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection)
{
	// 방향 구하기, 
	OutDirection = Camera->GetForwardVector();
	FTransform transform = Camera->GetComponentToWorld();
	FVector cameraLocation = transform.GetLocation();

	OutStart = cameraLocation + OutDirection;
	FVector conDirection = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInDegrees(OutDirection, 0.2f, 0.3f);
	conDirection *= 3000.0f;
	OutEnd = cameraLocation + conDirection;
}





ECombat_Type AGameActor::Get_Combat()
{
	return Combat_component->Get_CurrentCombat(); 
}
UParticleSystemComponent* AGameActor::Get_AttachParticle()
{
	return Attach_Particle;
}

void AGameActor::KnockBack_AddOffset(AActor* Attacker)
{
	CheckNull(Attacker);
	if(knockback)
	{
		FVector Direction = Attacker->GetActorForwardVector();
		AddActorWorldOffset(Direction.GetSafeNormal() * Last_Knockback);
	}
}

FRotator AGameActor::LookAt_Target()
{
	if(!!Target_Actor)
	{
		// Target Look At Rotation Return
		return UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target_Actor->GetActorLocation());
	}
	//CLog::Print("Target_Actor is Null");
	return FRotator();
}
FRotator AGameActor::ReverseLookAt_Target()
{	// Enemy Custom LookAt Player
	if (!!Target_Actor)
	{
		// Target Look At Rotation Return
		return UKismetMathLibrary::FindLookAtRotation( Target_Actor->GetActorLocation(), GetActorLocation());
	}
	//CLog::Print("Target_Actor is Null");
	
	return FRotator();
}

ACThrow_AttachDummy* AGameActor::Get_ThrowDummy()
{
	if(!!Throw_Dummy)
		return Throw_Dummy;
	return nullptr;
}

void AGameActor::TakeOverlap_SetAttackTag(EAttackTag_Type AttackTag, float power)
{
	Last_AttackTag = AttackTag;
	Last_Knockback = power;
}

void AGameActor::Add_Item(int id)
{
	FInven_Item newitem;
	//newitem.id = id;
	inven_item.Add(newitem);
}

void AGameActor::Remove_Item(int id)
{
	int i = 0;
	for (i = 0; i < inven_item.Num(); i++)
	{
		if (inven_item[i].id == id)
		{	// ID와 일치하는 idx 찾음
			inven_item.RemoveAt(i);
			break;
		}
	}
}

TArray<FInven_Item> AGameActor::Has_Item()
{
	return inven_item; 
}

EDirection AGameActor::TargetDirection(AGameActor* Target)
{
	EDirection direction;
	float dot_Result;
	FVector Cross_Result;

	dot_Result = UKismetMathLibrary::Dot_VectorVector(GetOwner()->GetActorForwardVector(), Target->GetActorForwardVector());
	Cross_Result = UKismetMathLibrary::Cross_VectorVector(GetOwner()->GetActorForwardVector(), Target->GetActorForwardVector());
	

	bool Forward_Chk, Right_Chk;

	Forward_Chk = dot_Result < 0 ? true : false; // 정면에서 맞음

	if (-0.5f < Cross_Result.Z && Cross_Result.Z < 0.5f)
	{
		/*if (Forward_Chk)
			CLog::Print("Front");
		else
			CLog::Print("Back");*/
		direction = Forward_Chk ? EDirection::Front : EDirection::Back;
	}
	else
	{
		Right_Chk = 0 < Cross_Result.Z ? true : false;
		/*if (Right_Chk)
			CLog::Print("Left");
		else
			CLog::Print("Right");*/
		direction = Right_Chk ? EDirection::Left : EDirection::Right;
	}
	return direction;
}

void AGameActor::ParryingMontage(EDirection direction)
{
	Combat_component->ParryingMontage(direction);
}

void AGameActor::BeParriedMontage(EDirection direction, ACharacter* ActionActor)
{
	Combat_component->BeParriedMontage(direction, ActionActor);

}


void AGameActor::Counterable(bool val)
{
	ParryingBoxComp->SetGenerateOverlapEvents(val);
}

