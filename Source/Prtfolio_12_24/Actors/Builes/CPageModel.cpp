#include "CPageModel.h"
#include "Global.h"

ACPageModel::ACPageModel()
{
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &MeshComp, "MeshComp");
	
	CHelpers::GetAsset<USkeletalMesh>(&Mesh, L"SkeletalMesh'/Game/Shop_Asset/InteractiveBook/Mesh/SkeletonMesh/SK_Page.SK_Page'");

	MeshComp->SetSkeletalMesh(Mesh);
}



void ACPageModel::BeginPlay()
{
	Super::BeginPlay();
	
}
