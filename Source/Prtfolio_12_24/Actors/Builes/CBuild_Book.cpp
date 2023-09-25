#include "CBuild_Book.h"
#include "Global.h"
#include "CBuild.h"
#include "CPageMaster.h"
#include "ActorComponents/Combat_Component/CPageCombatComponent.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimSingleNodeInstance.h"

ACBuild_Book::ACBuild_Book()
{
	LinkNode = new UDLinkNode<class ACBuild*>();
	LinkNode2 = new UDLinkNode<ACPageMaster*>();
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &mesh, "mesh");

	USkeletalMesh* Mesh;
	CHelpers::GetAsset<USkeletalMesh>(&Mesh, L"SkeletalMesh'/Game/Shop_Asset/InteractiveBook/Mesh/SkeletonMesh/SK_Cover.SK_Cover'");
	mesh->SetSkeletalMesh(Mesh);
	mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	
}


void ACBuild_Book::BeginPlay()
{
	Super::BeginPlay();

	ACBuild* build = GetWorld()->SpawnActor<ACBuild>();//����?
	LinkNode->PushNode(build);

	mesh->SetVisibility(false);
	PageCover =  NewObject<ACPageMaster>();
	PageCover = GetWorld()->SpawnActor<ACPageMaster>(PageCover->GetClass());

	LinkNode2->PushNode(PageCover);
	PageCover->Page_In_Book(this);
	
	
	//PageCover->SetFront(false);
}

void ACBuild_Book::Open_BuildBook(USceneComponent* parent)
{
	if (!(combat == EBookCombat::Open) && !mesh->GetVisibleFlag()) // �ȿ������� ��
	{
		combat = EBookCombat::Open;
		mesh->SetVisibility(true);
		
		AttachToComponent(parent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		mesh->SetRelativeLocation(FVector(0, 0, 0));
		OpenBook_Update();
	}
	else if(combat == EBookCombat::Open && mesh->GetVisibleFlag()) //�������� ��
	{
		PageCover->CombatComp->Set_InvisibleMode();
		for (int i = 0; i < LinkNode2->size; i++)
		{
			ACPageMaster* page = LinkNode2->GetNode(i)->Data;
			if (!!page)
				page->CombatComp->Set_InvisibleMode();
		}
		combat = EBookCombat::Close;
		
		mesh->SetRelativeLocation(FVector(0, 0, 400));
		mesh->SetVisibility(false);
	}
}

void ACBuild_Book::Page_In_Book( ACPageMaster* page)
{
	
	LinkNode2->PushNode(page);
	LinkNode->PushNode(page->Get_Build());

	page->CombatComp->Set_InvisibleMode();

	page->Page_In_Book(this);
	page->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
	page->SetActorLocation(GetActorLocation());
}

void ACBuild_Book::LookPage_Change(bool NextPage)
{
	
	if (0 <= LookPage && LookPage <= LinkNode2->size && combat == EBookCombat::Open) 
	{
		Page_Update(NextPage);
	}

	if (NextPage)
	{
		/*if (LinkNode2->size == LookPage)
			combat = EBookCombat::Close;*/
		
		if (LinkNode2->size > LookPage) 
		{
			/*for (int i = 0; i < LinkNode2->size; i++)
				LinkNode2->GetNode(i)->Data->CombatComp->Set_InvisibleMode();
			LinkNode2->GetNode(0)->Data->CombatComp->Set_S_BackMode();*/
			LookPage++;
		}
	}
	else 
	{
		if (LookPage > 0)
			LookPage--;
		if (combat == EBookCombat::Close)
		{
			combat = EBookCombat::Open;
			for(int i =0; i < LinkNode2->size ; i ++)
				LinkNode2->GetNode(i)->Data->CombatComp->Set_InvisibleMode();
			LookPage = 0;
			LinkNode2->GetNode(0)->Data->CombatComp->Set_S_FrontMode();
		}
		
	}
	
}


void ACBuild_Book::OpenBook_Update()
{
	if (LinkNode2->size > LookPage)
	{
		if (LookPage == 0)
			PageCover->CombatComp->Set_S_FrontMode();
		else
		{
			LinkNode2->GetNode(LookPage - 1)->Data->Init_MeshLocation(this);
			LinkNode2->GetNode(LookPage - 1)->Data->CombatComp->Set_BackMode();
		}
	}
	if (LinkNode2->size > LookPage )
	{
		LinkNode2->GetNode(LookPage )->Data->Init_MeshLocation(this);
		LinkNode2->GetNode(LookPage )->Data->CombatComp->Set_S_FrontMode();
	}
}

void ACBuild_Book::Select_Build()
{

	if(!!(LinkNode->GetNode(LookPage)->Data) && LookPage != 0 && LookPage < LinkNode2->size)
	{
		ACBuild* temp = GetWorld()->SpawnActor<ACBuild>(LinkNode->GetNode(LookPage)->Data->GetClass());
		temp->Set_Onwer(Player);
	}
}

void ACBuild_Book::Page_Update(bool NextPage)
{
	
	for (int i = LookPage; i < LinkNode2->size - 1; i++)
		LinkNode2->GetNode(i)->Data->CombatComp->Set_InvisibleMode();
	if (NextPage)  // ������
	{
		if (LinkNode2->size > LookPage)
		{
			if (LinkNode2->size > LookPage + 1)
			{
				LinkNode2->GetNode(LookPage + 1)->Data->Init_MeshLocation(this);
				LinkNode2->GetNode(LookPage + 1)->Data->CombatComp->Set_S_FrontMode();
				
			}

			if (LookPage == 0)
			{
				PageCover->CombatComp->Set_BackMode();
				
			}
			else
			{	
				LinkNode2->GetNode(LookPage)->Data->Init_MeshLocation(this);
				LinkNode2->GetNode(LookPage)->Data->CombatComp->Set_BackMode();
				
			}
			
		}
		
		
	}
	else // �ݴ� ����
	{
		if (LookPage == 1 || LookPage == 0) //ù������
		{
			PageCover->CombatComp->Set_FrontMode();
			if(LinkNode2->size >= 2)
				LinkNode2->GetNode(1)->Data->CombatComp->Set_S_FrontMode();
		}
		else if (LookPage == LinkNode2->size) //�������������� �������� �����������
		{
			LinkNode2->GetNode(LookPage - 1)->Data->CombatComp->Set_FrontMode();
		}
		else if (LookPage - 1 >= 0) 
		{
			if(LinkNode2->size > LookPage)
				LinkNode2->GetNode(LookPage - 1)->Data->CombatComp->Set_FrontMode();
			//LinkNode2->GetNode(LookPage)->Data->CombatComp->Set_S_FrontMode();

		}
		UKismetSystemLibrary::K2_SetTimer(this, "Delay_Invisible", 0.7f, false); // ������ �Լ��� �����Ͽ� �� �� invisible
		
		
	}
}

void ACBuild_Book::Delay_Invisible()
{
	if(LookPage + 1 < LinkNode2->size)
		LinkNode2->GetNode(LookPage + 1)->Data->CombatComp->Set_InvisibleMode();
}
