#include "Cameras/CCamera_Manager.h"
#include "Global.h"
#include "Cameras/CFallow_Camera.h"
#include "Cameras/CDead_Camera.h"
#include "Actors/CPlayer.h"
#include "ActorComponents/State_Component/CStateComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

ACCamera_Manager::ACCamera_Manager()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ACCamera_Manager::BeginPlay()
{
	Super::BeginPlay();

	AActor* player_temp;
	player_temp = UGameplayStatics::GetActorOfClass(GetWorld(),ACPlayer::StaticClass());
	
	OwnerCharacter = Cast<ACharacter>(player_temp);
	FTransform trans;
	//Fallow_Camera = GetWorld()->SpawnActor<ACFallow_Camera>(Fallow_Camera_class, trans);
	Fallow_Camera  = GetWorld()->SpawnActorDeferred<ACFallow_Camera>(Fallow_Camera_class, trans);
	Fallow_Camera->FinishSpawning(trans);
	Dead_Camera = GetWorld()->SpawnActor<ACDead_Camera>(Dead_Camera_class, trans);
	
	Skill_Camera = GetWorld()->SpawnActorDeferred<ACDead_Camera>(Skill_class[0], trans);
	Skill_Camera->FinishSpawning(trans);
	
	player = Cast<ACPlayer>(OwnerCharacter);

	if (!!player) 
	{

		OwnerCamera = CHelpers::GetComponent<UCameraComponent>(player);
		Fallow_Camera->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative,true));
		player->CameraDelegate_H.AddDynamic(Fallow_Camera, &ACFallow_Camera::Camera_HRotation);
		player->CameraDelegate_V.AddDynamic(Fallow_Camera, &ACFallow_Camera::Camera_VRotation);
		Dead_Camera->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		Skill_Camera->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	}
	if (!!OwnerCharacter && !!Fallow_Camera)
		Fallow_Camera->Set_OwnerCharacter(OwnerCharacter);
	if (!!OwnerCharacter && !!Dead_Camera)
		Dead_Camera->Set_OwnerCharacter(OwnerCharacter);
	if (!!OwnerCharacter && !!Skill_Camera)
		Skill_Camera->Set_OwnerCharacter(OwnerCharacter);
}

void ACCamera_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!!player)
	{
		RootComponent->SetWorldLocation(player->GetActorLocation());
		//RootComponent->SetWorldRotation(player->GetActorRotation());
	}
}

void ACCamera_Manager::Set_View_Fallow_Camera()
{
	if (!!Fallow_Camera) 
		Fallow_Camera->Set_View_Fallow_Camera();
	else
		CLog::Log("Fallow_Camera is Null");
}

void ACCamera_Manager::Set_View_Owner_Camera()
{
	
	if (!!player)
	{
		UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(player);
		if(!!State)
			State->Set_Move();
		APlayerController* control = Cast<APlayerController>(player->Controller);
		if (!!control)
			control->SetViewTarget(OwnerCamera->GetOwner());
		else
			CLog::Log("is not Player Controll _ CameraManager");
	}
}

void ACCamera_Manager::Set_View_Dead_Camera()
{
	if (!!Dead_Camera)
		Dead_Camera->Set_View_Dead_Camera();
	else
		CLog::Log("DeadCamera is Null");
	
}

void ACCamera_Manager::Set_View_Skill_Camera()
{
	if (!!Skill_Camera)
		Skill_Camera->Set_View_Dead_Camera();
	else
		CLog::Log("DeadCamera is Null");

}

FRotator ACCamera_Manager::Get_Fallow_Camera_Rotation()
{
	return Fallow_Camera->Get_Fallow_Camera_Rotation();
}
void ACCamera_Manager::Set_Fallow_Camera_Rotation(FRotator rot)
{

	return Fallow_Camera->Set_Fallow_Camera_Rotation(rot);
}


void ACCamera_Manager::DrawLine_Forward()
{
	Fallow_Camera->DrawLine_Forward();
}
