#include "Cameras/CDead_Camera.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Actors/CPlayer.h"

#include "ActorComponents/State_Component/CStateComponent.h"


ACDead_Camera::ACDead_Camera()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USpringArmComponent>(this, &Spring, "Spring", Root);
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", Spring);
	RootComponent = Root;

	Spring->TargetArmLength = 280.0f;
	Spring->bDoCollisionTest = false;
	Spring->bUsePawnControlRotation = true;
	Spring->bEnableCameraLag = true;
	Spring->SetRelativeLocation(FVector(0, 0, 40));
}

// Called when the game starts or when spawned
void ACDead_Camera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACDead_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACDead_Camera::Set_OwnerCharacter(ACharacter* inOwner)
{
	OwnerCharacter = inOwner;
	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	if (!!State)
	{
		State->Set_IdleMode();
		State->Set_Move();
	}
}


void ACDead_Camera::Set_View_Dead_Camera()
{

	player = Cast<ACPlayer>(OwnerCharacter);
	if (!!player)
	{
		APlayerController* control = Cast<APlayerController>(player->Controller);
		if (!!control)
		{
			control->SetViewTarget(this);
			View_DeadCamera();
		}
		else
			CLog::Log("is not Player Controll");
	}
	



}

