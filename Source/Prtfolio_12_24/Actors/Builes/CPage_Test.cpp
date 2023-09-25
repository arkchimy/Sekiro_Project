#include "CPage_Test.h"
#include "Global.h"
#include "CPageModel.h"
#include "CModel.h"
#include "CModelFactory.h"



ACPage_Test::ACPage_Test()
{
	
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh");
	
}

void ACPage_Test::BeginPlay()
{
	Super::BeginPlay();
	//ACWorld* world = Cast<ACWorld>(UGameplayStatics::GetActorOfClass(GetWorld(), ACWorld));
	ACWorld* World = Cast<ACWorld>(UGameplayStatics::GetActorOfClass(GetWorld(), ACWorld::StaticClass()));
	ACModelFactory* factory = World->Get_ModelFactory();
	Mesh->SetSkeletalMesh(factory->GetModel(EModelType::Page)->Get_Mesh());

}


