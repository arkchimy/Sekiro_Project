#include "CActorComponent_Target.h"
#include "ActorComponents/State_Component/CStateComponent.h"

#include "GameFrameWork/Character.h"
#include "Global.h"

#include "Actors/CPlayer.h"

#include "Utilities/CEnums.h"


#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
UCActorComponent_Target::UCActorComponent_Target()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UCActorComponent_Target::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	player = Cast<ACPlayer>(OwnerCharacter);
	

}


void UCActorComponent_Target::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!!Target_Actor)
	{
		float dot_result = UKismetMathLibrary::Dot_VectorVector(GetOwner()->GetActorForwardVector(), Target_Actor->GetActorForwardVector());
		bBackPos = dot_result > 0;
	}
	else
		bBackPos = false;
	CheckNull(Target_Actor); // target이 없다면 return
	
	

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Target_Actor);


	bool b = false;
	b |= state->IsDeathState();
	b |= Target_Actor->GetDistanceTo(OwnerCharacter) >= TraceRadius;
	if(b)
	{
		End_Targeting();
		return;
	}
		
	
	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = Target_Actor->GetActorLocation();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	FRotator current = OwnerCharacter->GetControlRotation();

	
	rotator = UKismetMathLibrary::RInterpTo(current, FRotator(current.Pitch,rotator.Yaw,rotator.Roll), DeltaTime, InterpSpeed);
	
	
	state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

	//CheckTrue(state->IsActionState()); // Action중에는 적용 안됌.
	if(!!player)  // player인 경우
	{
		ECamera_Sight sight = player->Get_Sight();
		switch(sight)
		{
		case ECamera_Sight::TPS:
			player->Set_Fallow_Camera_Rotation(rotator);
		case ECamera_Sight::FPS :
			OwnerCharacter->GetController()->SetControlRotation(rotator);
		}
	}
	else // Monster 인 경우
	{
		OwnerCharacter->GetController()->SetControlRotation(rotator);
	}

	
}

void UCActorComponent_Target::Find_Target()
{

	
	CheckNull(OwnerCharacter);
	
	FVector start, end;
	start = OwnerCharacter->GetActorLocation();
	
	end = FVector(start.X, start.Y, start.Z + 1);
	//end = start + OwnerCharacter->GetActorLocation().ForwardVector.GetSafeNormal() * 300;

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	TArray<TEnumAsByte<EObjectTypeQuery>> obj_Types;
	obj_Types.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),start,end,TraceRadius, obj_Types,true,ignoreActors,EDrawDebugTrace::ForOneFrame,hitResults,true,FLinearColor::Green, FLinearColor::Red, 5.f);
	
	CheckFalse(hitResults.Num()); // 0이면 False
	CheckFalse(hitResults[0].bBlockingHit);
	AGameActor* target = Cast<AGameActor>(hitResults[0].Actor); //  선정된 target
	AGameActor* temp = target;   //  비교를 위한 temp;
	float distance = 0;
	if (!!Target_Actor) 
	{
		End_Targeting();
		return;
	}
	else 
	{
		for (int i = 1; i < hitResults.Num(); i++)
		{
			if (!!target)
			{
				temp = Cast<AGameActor>(hitResults[i].Actor);
				distance = OwnerCharacter->GetDistanceTo(target);
				if (OwnerCharacter->GetDistanceTo(temp) < distance)
					target = temp; // 가장 가까운 target으로 교체
			}
		}
		Target_Actor = target; // 가장 가까운 Character
	}


	Start_Targeting();
}

void UCActorComponent_Target::Skill_Look_Start()
{
	if (!!Target_Actor)
		return ;
	Find_Target();
	bSkiil_Mode = true; 
}
void UCActorComponent_Target::Skill_Look_End()
{
	
	//CheckFalse(bSkiil_Mode);
	End_Targeting();
}


void UCActorComponent_Target::Start_Targeting()
{
	bTarget_Mode = true;
	CheckNull(Particle);
	
	CheckNull(OwnerCharacter);
	CheckNull(Target_Actor);
	Owner_Actor = Cast<AGameActor>(OwnerCharacter);
	Owner_Actor->View_EnemyUI(Target_Actor);
	Attached = UGameplayStatics::SpawnEmitterAttached(Particle, Target_Actor->GetMesh(), "Spine_Target");

}

void UCActorComponent_Target::End_Targeting()
{
	bTarget_Mode = false;
	Target_Actor = nullptr;
	if (!!Attached)
		Attached->DestroyComponent();
	Owner_Actor->View_EnemyUI(Target_Actor);
}
