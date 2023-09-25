#include "CBuild.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "Particles/ParticleSystem.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Actors/CThrowActors/CThrow_AttachDummy.h"
#include "Components/BoxComponent.h"

#include "Sound/SoundCue.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACBuild::ACBuild()
{
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &mesh,"Mesh");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Create_mesh, "Create_mesh");
	
	RootComponent = mesh;


	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle_Comp, "Particle", Create_mesh);
	CHelpers::CreateComponent<UBoxComponent>(this, &Trigger_Mesh, "trigger_Mesh", Particle_Comp);
	//CHelpers::CreateComponent<UParticleSystemComponent>(this, &efft, "particle", Create_mesh);
	
	PrimaryActorTick.bCanEverTick = true;

	mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel10);
	Create_mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel10);
	Trigger_Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel7);


	//Create_mesh->SetVisibility(false);
}

void ACBuild::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	CheckFalse(state == EBuildState::Before);
	CheckTrue(OtherComp == Create_mesh);
	OverlapActors.AddUnique(OtherActor);

	mesh->SetMaterial(0, ImpossibleMat);
	bpossible = false;
	
}

void ACBuild::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	CheckFalse(state == EBuildState::Before);
	CheckTrue(OtherComp == Create_mesh);

	if(OverlapActors.Find(OtherActor) != -1)// 검사.
		OverlapActors.Remove(OtherActor);
	if (OverlapActors.Num() == 0) 
	{
		mesh->SetMaterial(0, possibleMat);
		bpossible = true;
	}
	
	
}


void ACBuild::Particle_Call()
{
	if (effects.Num() > 0)
	{
		if (!bParticle_chk) 
		{
			Particle_Comp->SetTemplate(effects[0].particle);
			Particle_Comp->SetVisibility(true);
			Paticle_Tag = effects[0].Tag;
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), effects[0].Particle_Sound, RootComponent->GetComponentLocation());
			bParticle_chk = !bParticle_chk;
			Trigger_Mesh->SetGenerateOverlapEvents(true);
		}
		else
		{
			Particle_Comp->SetVisibility(false);
			Paticle_Tag = FName("None");
			bParticle_chk = !bParticle_chk;
			Trigger_Mesh->SetGenerateOverlapEvents(false);
		}
	}
}

void ACBuild::Complete_Location()
{
	
	state = EBuildState::Ready;
	mesh->SetMaterial(0,ReadyMat);
	if(!!OwnerCharacter)
		OwnerCharacter->Set_Interaction_Build(nullptr);
	CheckNull(Create_mesh);
	

}

void ACBuild::Create_Build()
{
	CheckFalse(state == EBuildState::Ready);
	state = EBuildState::After; // 완성
	UStaticMesh* temp = nullptr;
	mesh->SetStaticMesh(temp);

	mesh->SetMaterial(0, CreateMat);
	Create_mesh->SetRelativeLocation(mesh->GetRelativeLocation());
	Create_mesh->SetRelativeRotation(mesh->GetRelativeRotation());
	mesh->SetWorldLocation(FVector(0, 0, 0));


	Create_mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel10);
	Create_mesh->SetStaticMesh(Build_mesh);
	Create_mesh->SetVisibility(true);


	Create_mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	

}






void ACBuild::BeginPlay()
{
	Super::BeginPlay();


	mesh->SetVisibility(true);

	mesh->SetStaticMesh(Build_mesh);
	
	mesh->SetWorldLocation(FVector(0, 0, 0));
	mesh->SetRelativeLocation(FVector(0, 0, 0));
	
	
	

	UMaterialInstanceConstant* Green_color;
	UMaterialInstanceConstant* Red_color;
	UMaterialInstanceConstant* white_color;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&Red_color, "MaterialInstanceConstant'/Game/Materials/Impossible_Mat_Inst.Impossible_Mat_Inst'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&Green_color, "MaterialInstanceConstant'/Game/Materials/possible_Mat_Inst.possible_Mat_Inst'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&white_color, "MaterialInstanceConstant'/Game/Materials/Ready_Mat_Inst.Ready_Mat_Inst'");


	possibleMat = UMaterialInstanceDynamic::Create(Green_color, this);
	ImpossibleMat = UMaterialInstanceDynamic::Create(Red_color, this);
	ReadyMat = UMaterialInstanceDynamic::Create(white_color, this);

	CreateMat = mesh->GetMaterial(0);
	mesh->SetMaterial(0,possibleMat);

	mesh->OnComponentBeginOverlap.AddDynamic(this, &ACBuild::OnComponentBeginOverlap);
	mesh->OnComponentEndOverlap.AddDynamic(this, &ACBuild::OnComponentEndOverlap);
	Trigger_Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACBuild::Overlap_Event);
	
}

void ACBuild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(state == EBuildState::Before)// 자리 지정중이라면
	{
		//마우스의 위치를 따라가게 해야함.
		//BP로 해보자.
		if(!!OwnerCharacter && !bhasOwner)
		{
			bhasOwner = true;
			OwnerCharacter->Set_Interaction_Build(this);

			//Select_location(); //player 가 있을때만 적용
		}
	}
}

void ACBuild::Overlap_Event(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	CheckFalse(Particle_Comp->GetVisibleFlag());//꺼져있으면 취소.
	
	ACThrow_AttachDummy* dummy = Cast<ACThrow_AttachDummy>(OtherActor);
	
	if(!!dummy) // Throw_Dummy 일경우 Tag체크
	{
		
		dummy->Trigger_TagChk(Paticle_Tag);
	}
	
	

}