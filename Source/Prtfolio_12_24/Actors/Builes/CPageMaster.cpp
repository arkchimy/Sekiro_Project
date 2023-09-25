#include "Actors/Builes/CPageMaster.h"
#include "Global.h"
#include "Components/BoxComponent.h"
#include "CBuild_Book.h"
#include "CModelFactory.h"
#include "CPageModel.h"
#include "CModel.h"
#include "ActorComponents/Combat_Component/CPageCombatComponent.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Prtfolio_12_24GameModeBase.h"





ACPageMaster::ACPageMaster()
{
	
	
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &mesh, "Mesh");
	CHelpers::CreateComponent<UBoxComponent>(this, &collision, "Box",mesh);
	CHelpers::CreateActorComponent<UCPageCombatComponent>(this, &CombatComp, "CombatComponent");

	CHelpers::GetAsset<UMaterialInstanceConstant>(&mesh_mat, L"MaterialInstanceConstant'/Game/Shop_Asset/InteractiveBook/Examples/Pages/Materials/MI_Page_Example_Empty_02.MI_Page_Example_Empty_02'");
	mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	
	TSubclassOf<UAnimInstance> anim = nullptr;
	CHelpers::GetClass<UAnimInstance>(&anim, L"AnimBlueprint'/Game/_Objects/Map_Object/Builds/ABC_PageAniminstance.ABC_PageAniminstance_C'");
	if (!!anim)
	{
		mesh->SetAnimInstanceClass(anim);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load animation instance"));
	}


	collision->SetBoxExtent(FVector(0.6f, 15, 20));
	
	
	RootComponent = mesh;
	
	mesh->SetRelativeLocation(FVector(0, 0, 0));

	FTransform trans;
	trans.SetLocation(FVector(0, 0, 0));
	mesh->SetRelativeTransform(trans);


	mesh->SetMaterial(0, mesh_mat);
}

ACPageMaster::~ACPageMaster()
{
	
}

void ACPageMaster::Page_In_Book(ACBuild_Book* book)
{
	SetActorRotation(FQuat(FRotator(0, 0, 0)));
	OwnerBook = book;
	CombatComp->Set_InvisibleMode();
	if(!!OwnerBook)
		AttachToActor(OwnerBook, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));

}

void ACPageMaster::Page_Update(bool NextPage)
{
	if(!!OwnerBook)
	{
		OwnerBook->Page_Update(NextPage);
	}
}

void ACPageMaster::Set_Build(class ACBuild* build)
{
	Obj_class = build;
}

void ACPageMaster::Init_MeshLocation(ACBuild_Book* book)
{

	FTransform trans;
	trans.SetLocation(FVector(0, 0, 0));
	mesh->SetRelativeTransform(trans);
	
	AttachToActor(book, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	
	mesh->SetRelativeLocation(FVector(0, 0, 0));

}

bool ACPageMaster::Is_PickMode()
{
	return CombatComp->Is_PickMode();
}

bool ACPageMaster::Is_S_BackMode()
{
	return CombatComp->Is_S_BackMode();
}
bool ACPageMaster::Is_S_FrontMode()
{
	return CombatComp->Is_S_FrontMode();
}

bool ACPageMaster::Is_BackMode()
{
	return CombatComp->Is_BackMode();
}

bool ACPageMaster::Is_FrontMode()
{
	return CombatComp->Is_FrontMode();
}

bool ACPageMaster::Is_InvisibleMode()
{
	return CombatComp->Is_InvisibleMode();
}

void ACPageMaster::BeginPlay()
{
	Super::BeginPlay();
	id = APrtfolio_12_24GameModeBase::Page_Cnt++;

	CombatComp->Set_Page(this);

	collision->SetRelativeLocation(FVector(0, -15, 0));
	dynamic_mat = mesh->CreateDynamicMaterialInstance(0, mesh->GetMaterial(0), "DynamicMat");

	ACWorld* World = Cast<ACWorld>(UGameplayStatics::GetActorOfClass(GetWorld(), ACWorld::StaticClass()));
	if (!!World) 
	{
		ACModelFactory* factory = World->Get_ModelFactory();
		mesh->SetSkeletalMesh(factory->GetModel(EModelType::Page)->Get_Mesh());
		mesh->SetMaterial(0, dynamic_mat);
		ID_init(id);
	}
	
}




