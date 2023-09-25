#include "Cameras/CFallow_Camera.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Actors/CPlayer.h"

#include "ActorComponents/State_Component/CStateComponent.h"

#include "Kismet/KismetSystemLibrary.h"
ACFallow_Camera::ACFallow_Camera()
{
	PrimaryActorTick.bCanEverTick = true;


	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USpringArmComponent>(this, &Spring, "Spring",Root);
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", Spring);
	RootComponent = Root;

	Spring->TargetArmLength = 300.f;
	Spring->bDoCollisionTest = false;
	Spring->bUsePawnControlRotation = true;
	Spring->bEnableCameraLag = true;
	Spring->SetRelativeLocation(FVector(0, 50, 30));
	
}

void ACFallow_Camera::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACFallow_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Spring->SetRelativeRotation(FRotator(Spring->GetRelativeRotation().Pitch, Spring->GetRelativeRotation().Yaw, 0)); // Roll 고정
	if(Used)
		Spring->SetRelativeRotation(FRotator(Spring->GetRelativeRotation().Pitch, Spring->GetRelativeRotation().Yaw, 0)); // Roll 고정
	else if(!!player)
		Spring->SetRelativeRotation(FRotator(player->GetActorRotation()));

	//Spring->SetRelativeLocation(FVector(0, 0, 110));
	Spring->SetRelativeLocation(FVector(0, 50, 30));
}


void ACFallow_Camera::Set_OwnerCharacter(ACharacter* inOwner)
{
	OwnerCharacter = inOwner;
	player = Cast<ACPlayer>(OwnerCharacter);
	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);

	if (!!State) 
	{
		State->Set_IdleMode();
		State->Set_Move();
	}
}


void ACFallow_Camera::Set_View_Fallow_Camera()
{
	if(!!player)
	{
		APlayerController* control = Cast<APlayerController>(player->Controller);
		Spring->SetRelativeRotation(FRotator(-5,player->GetActorRotation().Yaw, player->GetActorRotation().Roll));
		
		if (!!control) 
		{
			control->SetViewTarget(this);
			
		}
		else
			CLog::Log("is not Player Controll");
	}

	Used = true;
	
}



void ACFallow_Camera::DrawLine_Forward()
{
	FVector Start = player->GetActorLocation();
	FRotator rot = Spring->GetRelativeRotation();
	FVector end = Start + FQuat(rot).GetForwardVector().GetSafeNormal2D() * 100;
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), Start, end,15.f, FLinearColor::Green);

}

FRotator ACFallow_Camera::Get_Fallow_Camera_Rotation()
{
	return Spring->GetRelativeRotation();
}

void ACFallow_Camera::Camera_HRotation(float Axis)
{
	FQuat rot = FQuat(FRotator(0, Axis, 0));
	Spring->AddRelativeRotation(rot);

}
void ACFallow_Camera::Set_Fallow_Camera_Rotation(FRotator rot)
{
	Spring->SetRelativeRotation(rot); 
}


void ACFallow_Camera::Camera_VRotation(float Axis)
{
	FQuat rot = FQuat(FRotator(Axis, 0, 0));
	bool Limit_chk  = false;
	if(0 < Axis)
		Limit_chk =  20 < Camera->GetRelativeRotation().Pitch;
	else
		Limit_chk =  Camera->GetRelativeRotation().Pitch < -10;
	
	if(!Limit_chk)	
		Camera->AddRelativeRotation(rot);
}

