#include "CPlayer.h"
#include "Global.h"



#include "Actors/Builes/CBuild_Book.h"
#include "Actors/Builes/CBuild.h"

#include "Actors/CThrowActors/CThrow_ManagerActor.h"

#include "Builes/CPageMaster.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


#include "Components/CapsuleComponent.h"

#include "Components/SceneCaptureComponent.h"

#include "ActorComponents/CStatusComponent.h"
#include "ActorComponents/State_Component/CStateComponent.h"
#include "ActorComponents/Combat_Component/CombatComponent.h"
#include "ActorComponents/CActorComponent_Particle.h"
#include "ActorComponents/CActorComponent_Target.h"

#include "Camera/CameraComponent.h"
#include "Cameras/CCamera_Manager.h"

#include "Engine/EngineTypes.h"

#include "Kismet/KismetSystemLibrary.h"

/*
* Widget class
*/
#include "Blueprint/UserWidget.h"
#include "UI_Inven/CUserWidget_Inventory.h"
#include "UI_Inven/CGame_HUD.h"
/*
* TreasureBox
*/
#include "Actors/TreasureBox/CTreasureBox_Master.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = GetCapsuleComponent();
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetupAttachment(RootComponent);
	

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &FP_Mesh, "Fp_Mesh", GetMesh());
	

	CHelpers::CreateActorComponent<UCActorComponent_Target>(this, &Target_Comp, "Target_Comp");
	


	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/_Meshes/Player/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/_Meshes/Player/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'");
	FP_Mesh->SetSkeletalMesh(mesh);

	FP_Mesh->SetRelativeLocation(FVector(0, 0, 0));
	FP_Mesh->SetRelativeRotation(FRotator(0, 0,-90));

	FP_Mesh->SetOnlyOwnerSee(true);
	FP_Mesh->CastShadow = false;
	FP_Mesh->bCastDynamicShadow = false;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -80));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	Camera->bUsePawnControlRotation = false;
	


	CHelpers::CreateActorComponent<USceneCaptureComponent>(this, &MeshWidget, "MeshWidget");
	CHelpers::CreateComponent<USceneComponent>(this, &Book_SceneComponent, "Book_Scene",Camera);
	
	Book_SceneComponent->SetRelativeLocation(FVector(50,0,0));
	Book_SceneComponent->SetRelativeRotation(FQuat(FRotator(180,180, 0)));

	
	Combat_component->ChangeType(ECombat_Type::Unarmed);
	
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	Init_Player();
	Playercontroll = Cast<APlayerController>(GetController());
	if (!!HUD_class) 
	{
		HUD = CreateWidget<UCGame_HUD>(Playercontroll, HUD_class);
		HUD->AddToViewport();
	}

	GetMesh()->SetRelativeLocation(FVector(0, 0,  GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) * -1);

	FP_Mesh->SetAnimInstanceClass(FP_animInstance);
	
	GetCharacterMovement()->MaxWalkSpeed = 600;
	Camera_Setting(ECamera_Sight::TPS);
}



void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	InteractionBuild_Update();
}

 


void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this, &ACPlayer::Move_Forward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::Move_Right);
	PlayerInputComponent->BindAxis("LookHorizontal", this, &ACPlayer::Look_Horizontal);
	PlayerInputComponent->BindAxis("LookVertical", this, &ACPlayer::Look_Vertical);
	
	PlayerInputComponent->BindAxis("MouseWheel", this, &ACPlayer::Build_Rotation);


	
	PlayerInputComponent->BindAction("Inven_Open", EInputEvent::IE_Pressed, this, &ACPlayer::Open_Inventory);

	PlayerInputComponent->BindAction("TPS_Camera_Rot", EInputEvent::IE_Pressed, this, &ACPlayer::Pressed_Alt);
	PlayerInputComponent->BindAction("TPS_Camera_Rot", EInputEvent::IE_Released, this, &ACPlayer::Released_Alt);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACPlayer::Move_Walk);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACPlayer::Move_Run);

	PlayerInputComponent->BindAction("Targetting", IE_Pressed, this, &ACPlayer::Find_Target);//Tab 입력
	PlayerInputComponent->BindAction("JumpKey", IE_Pressed, this, &ACPlayer::Input_SpaceBar);//Spacebar 입력

	PlayerInputComponent->BindAction("L_Button", EInputEvent::IE_Pressed, this, &ACPlayer::L_Attack);
	PlayerInputComponent->BindAction("L_Button", EInputEvent::IE_Released, this, &ACPlayer::L_AttackRelease);

	PlayerInputComponent->BindAction("R_Button", EInputEvent::IE_Pressed, this, &ACPlayer::R_Attack);
	PlayerInputComponent->BindAction("R_Button", EInputEvent::IE_Released, this, &ACPlayer::R_AttackRelease);

	PlayerInputComponent->BindAction("Build_Book", EInputEvent::IE_Pressed, this, &ACPlayer::Open_BuildBook);
	PlayerInputComponent->BindAction("BuildFill_Key", EInputEvent::IE_Pressed, this, &ACPlayer::Select_Build); //건물 완성시키기
	PlayerInputComponent->BindAction("PickUp", EInputEvent::IE_Pressed, this, &ACPlayer::PickUp);

	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed,this, &ACPlayer::OnFist);
	PlayerInputComponent->BindAction("SS", EInputEvent::IE_Pressed, this, &ACPlayer::OnSS);


	PlayerInputComponent->BindAction("Absolut", EInputEvent::IE_Pressed, this, &ACPlayer::Absolut_Attack);

}



void ACPlayer::Stren_Damaged(ACharacter* Attacker, FString CC_Option) //  무슨 함수인데?
{
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),Attacker->GetActorLocation());
	
	FVector location = Attacker->GetActorLocation() + Attacker->GetActorForwardVector() * 80.f;

	SetActorRotation(rotation);
	SetActorLocation(location);

	State->Set_StunMode();

}





void ACPlayer::Build_Rotation(float Axis)
{
	if (!!interact_Build)
	{
		CheckFalse(interact_Build->State() == EBuildState::Before);
		FRotator rotation = FRotator(0, interact_Build->GetActorRotation().Yaw + Axis, 0);
		interact_Build->SetActorRotation(rotation);
	}

}

void ACPlayer::Weapon_Change(ECombat_Type type)
{
	//Camera_Setting(ECamera_Sight::TPS);
	if (State->IsIdleState())
	{
		if (Combat_component->Get_CurrentCombat() == ECombat_Type::Unarmed)
		{

			Combat_component->ChangeType(type);
			Combat_component->Equip();
			bFight_Mode = true;
			
		}
		else if (Combat_component->Get_CurrentCombat() == type)
		{

			Combat_component->UnEquip();
			Combat_component->ChangeType(ECombat_Type::Unarmed);
			CLog::Print("UnEquip Called");
			bFight_Mode = false;
		}
		else
		{
			Combat_component->Set_SwapCombat(type);
			Combat_component->UnEquip();//현재 무기 해제.

		}
		
	}
}

void ACPlayer::Find_Target()
{
	Target_Comp->Find_Target();
}

void ACPlayer::Input_SpaceBar()
{
	CheckFalse(State->IsIdleState());
	Play_Action(LastKey, EKeybord::SpaceBar);

}

void ACPlayer::OnFist(){ Weapon_Change(ECombat_Type::Fist); }
void ACPlayer::OnSS(){ Weapon_Change(ECombat_Type::SS); }
void ACPlayer::OnArcher(){ Weapon_Change(ECombat_Type::Archer); }
void ACPlayer::OnWizard(){ Weapon_Change(ECombat_Type::Wizard); }


void ACPlayer::Open_Inventory()
{
	HUD->Open_Inventory();
}

void ACPlayer::Open_TreasureBox(TArray<FInven_Item> Treasureitems, class ACTreasureBox_Master* box)
{
	HUD->Open_TreasureBox(Treasureitems,box);

}
void ACPlayer::Close_TreasureBox()
{
	HUD->Close_TreasureBox();
}

void ACPlayer::Open_BuildBook()
{
	if (State->IsIdleState()) //idle 상태일 때 열림.
	{
		build_book->Open_BuildBook(Book_SceneComponent);
		Set_View_Owner_Camera();
		State->Set_InteractionMode();
	}
	else if(State->IsInteractionState()) //이미 보고있다면
	{
		build_book->Open_BuildBook(Book_SceneComponent);
		Camera_Setting(Combat_component->Get_Sight());
		State->Set_IdleMode();
	}
}

void ACPlayer::Select_Build()
{
	CLog::Print("Select Build");
	if (State->IsInteractionState()) //책을 열어보고 있다면.
	{	//선택시 책 내리기. 

		State->Set_IdleMode();
		build_book->Select_Build();
		build_book->Open_BuildBook(Book_SceneComponent); //  4/20 프라시아
	}
	else 
	{
		FVector Start = Camera->GetComponentLocation();
		FVector End = Camera->GetComponentLocation() + Camera->GetForwardVector() * 500;


		FCollisionQueryParams params;

		params.AddIgnoredActor(this);

		FHitResult hitResult;
		GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_WorldStatic, params);
		//DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Red, false, 5.f, 0, 2.f);

		CheckFalse(hitResult.bBlockingHit);
		ACBuild* build_obj = Cast<ACBuild>(hitResult.GetActor());
		if (!!build_obj)
		{
			if (build_obj->State() == EBuildState::Ready)
				build_obj->Create_Build();
			else if (build_obj->State() == EBuildState::After)
				build_obj->Particle_Call();
		}
	}
	
}





void ACPlayer::PickUp()
{
	FVector Start = Camera->GetComponentLocation();
	FVector Direction = Camera->GetForwardVector().GetSafeNormal();
	FVector End = Camera->GetComponentLocation() + Direction * 500;

	FCollisionQueryParams params;

	params.AddIgnoredActor(this);
	
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_WorldDynamic, params);
	//DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Red, false, 5.f, 0, 2.f);

	CheckFalse(hitResult.bBlockingHit);
	ACPageMaster* page = Cast<ACPageMaster>(hitResult.GetActor());
	if (!!page) 
	{
		if(page->Is_PickMode())
			build_book->Page_In_Book(page);
	}
	ACTreasureBox_Master* TreasureBox = Cast<ACTreasureBox_Master>(hitResult.GetActor());
	if (!!TreasureBox) 
	{	// Treasure box에 E클릭 했을떄.
		TreasureBox->Set_TouchActor(this);
		TreasureBox->Open_TreasureBox();
	}
	ACWeaponment* onFloor_Item = Cast<ACWeaponment>(hitResult.GetActor());
	if(!!onFloor_Item)
	{
		CheckFalse(onFloor_Item->IsFloorItem());
		
		bool bSpace = HUD->Item_PickUp(onFloor_Item->Get_Id());
		if (bSpace) 
		{
			CLog::Print("Insert Item");
			Add_Item(onFloor_Item->Get_Id());
			onFloor_Item->Destroy();
		}
		
	}
}



float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!!effect.Effect)
	{
		FTransform trans;
		trans.SetScale3D(effect.EffectTransform.GetScale3D());
		trans.SetLocation(effect.EffectTransform.GetLocation() + GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect.Effect, trans, true);

	}

	if(!State->IsBlockState())
		Status->Sub_Health(Damage);

	if (knockback)// 넉백이 가능하다면
		KnockBack_AddOffset(DamageCauser->GetOwner()); // 넉백 판정


	State->Hitted_PlayMontage(DamageCauser, Last_AttackTag);

	Damaged_Event();
	return 0.0f;
}
void ACPlayer::View_EnemyUI(AGameActor* targetactor)
{
	HUD->View_EnemyUI(targetactor);
}



void ACPlayer::Slot_Swap(ACWeaponment* Weaponment)
{
	Combat_component->Slot_Swap(Weaponment);
	Combat_component->Equip();
}

void ACPlayer::Player_Move(bool val)
{
	if (!val)
		State->Set_UnMove();
	else
		State->Set_Move();
}



void ACPlayer::Set_Interaction_Build(ACBuild* build)
{
	CheckFalse(interact_Build == nullptr);
	if (!!build)
		interact_Build = build; // 상호 Build 갱신  . 스택으로 해둘까...?
	else
		interact_Build = nullptr;
}

void ACPlayer::SetCrossHair(FCrossHairSet crosshair)
{
	HUDCrossHair(crosshair);
}







//=======================  함수
void ACPlayer::Init_Player()
{
	TeamID = FGenericTeamId(C_ID);
	if (!!BP_buildBook_Class)
	{
		build_book = GetWorld()->SpawnActor<ACBuild_Book>(BP_buildBook_Class);
		build_book->Set_Owner(this);
	}

	GetCharacterMovement()->MaxWalkSpeed = Status->Get_RunSpeed();
	GetCharacterMovement()->MaxFlySpeed = Status->Get_FlySpeed();

	if (!!ThrowDummy_class)
	{
		Throw_Dummy = GetWorld()->SpawnActor<ACThrow_AttachDummy>(ThrowDummy_class);
		Throw_Dummy->Set_Owner(this);

	}
	FTransform trans;
	trans.SetLocation(FVector(0, 0, 0));
	Throw_Manager = GetWorld()->SpawnActor<ACThrow_ManagerActor>(ThrowManager_class, trans);


	if (!!CameraManager_Class)
	{
		Camera_Manager = GetWorld()->SpawnActor<ACCamera_Manager>(CameraManager_Class, trans);
	}
	if (!!MeshWidget)
	{
		MeshWidget->ShowOnlyActors.Add(this);

	}
}



//=========================================== Movement ================================================

void ACPlayer::Set_Max_Speed(float inval)
{
	GetCharacterMovement()->MaxWalkSpeed = inval;
}

void ACPlayer::Forward_Rush(float Axis)
{

	Move_Forward(Axis);

}

void ACPlayer::Move_Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 400;

}
void ACPlayer::Move_Run()
{
	if (bFight_Mode && State->IsIdleState()) // 전투 모드 ,  스텝 할 수 있는 상태일떄 호출
		Play_Action(LastKey, EKeybord::Shift);
	else
		GetCharacterMovement()->MaxWalkSpeed = 600;

}

void ACPlayer::Move_Forward(float Axis)
{

	CheckFalse(State->CanMove());
	
	if (Axis == 0) LastKey = EKeybord::Forward;
	else 
		LastKey = 0 < Axis ? EKeybord::Forward : EKeybord::Back;
	

	FRotator rotator;
	FVector Direction;
	if (!Target_Comp->Is_TargetMode())
		GetCharacterMovement()->MaxWalkSpeed = 600; //Combat 에서 Speed 를 가져오거나 거기서 세팅해야할듯?

	rotator = FRotator(0, GetActorRotation().Yaw, 0);
	Direction = FQuat(rotator).GetForwardVector().GetSafeNormal2D();
	AddMovementInput(Direction, Axis);

}

void ACPlayer::Move_Right(float Axis)
{
	if(Axis != 0)
		LastKey = 0 < Axis ? EKeybord::Right : EKeybord::Left;
	CheckFalse(State->CanMove());

	FRotator rotator = FRotator(0, GetActorRotation().Yaw, 0);
	FVector Direction = FQuat(rotator).GetRightVector().GetSafeNormal2D();

	AddMovementInput(Direction, Axis);

}

void ACPlayer::Look_Horizontal(float Axis)
{
	if(sight == ECamera_Sight::FPS)
	{
		AddControllerYawInput(Axis);
	}
	CheckTrue(State->IsActionState()); // action중이면 화면 안돌아가가게 하기
	AddControllerYawInput(Axis);

}

void ACPlayer::Look_Vertical(float Axis)
{

	if (sight == ECamera_Sight::FPS)
	{
		AddControllerPitchInput(Axis);
	}

	CheckTrue(State->IsActionState());
	AddControllerPitchInput(Axis);
	/*if (sight == ECamera_Sight::FPS)
		AddControllerPitchInput(Axis);
	else if (sight == ECamera_Sight::TPS)
	{	
		if (CameraDelegate_V.IsBound())
			CameraDelegate_V.Broadcast(-Axis);
	}*/
}

void ACPlayer::Character_ForwardRotation(const FVector forward, float Axis)
{
	CheckTrue(Target_Comp->Is_TargetMode()); // TargetMode 면 회전안함

	if (-0.5f < forward.Z && forward.Z < 0.5f) {}
	else if (0.1 < forward.Z)
		AddControllerYawInput(Axis * 2.f);
	else if (forward.Z < -0.1f)
		AddControllerYawInput(-Axis * 2.f);
}

void ACPlayer::Character_Rotation(const float dot, const FVector Cross, float Axis)
{
	CheckTrue(Target_Comp->Is_TargetMode()); // TargetMode 면 회전안함
	if (0 < dot)
	{
		AddControllerYawInput(Axis * TurnSpeed);
	}
	else if (0 < Cross.Z && dot < 0) // 90도를 넘어가서  왼쪽을 보고있을떄
	{
		
		AddControllerYawInput(-Axis * TurnSpeed);
		
	}
	else if (Cross.Z < 0 && dot < 0) // 90도를 넘어가서 오른쪽을 보고있을때
	{
		
		AddControllerYawInput(-Axis * TurnSpeed);
		
	}
}


// =======================================   Mouse Button =============================================
void ACPlayer::L_Attack()
{
	L_But = true;
	if (!!interact_Build)
	{
		CheckFalse(interact_Build->State() == EBuildState::Before);
		CheckFalse(interact_Build->IsPossible());
		interact_Build->Complete_Location();
		interact_Build = nullptr;
		return;
	}

	if (State->IsInteractionState()) //책 보고있을 경우
		build_book->LookPage_Change(true);
	else if (State->IsBlockState()) 
	{
		//CLog::Print("BlockMode");
		Combat_component->Do_Parrying();
	}
	else if(Target_Dot())
	{	// Target의 뒤를 잡았다면
		
		Combat_component->SuddenAttack();
	}
	else if (!State->IsStunState()) //공격 불가 상태일 때
	{
		Combat_component->L_Attack();
	}
	else if (!State->IsActionState())// 잡기 당했을 때
	{
		resiste_Cnt++;

		CheckFalse(resiste_Cnt > 5);
		resiste_Cnt = 0;;
		
	}
}


void ACPlayer::L_AttackRelease()
{
	L_But = false;

	if (!State->IsStunState() && !State->IsInteractionState())
		Combat_component->L_AttackRelease();
}
void ACPlayer::R_Attack()
{
	R_But = true;

	if (State->IsInteractionState())
		build_book->LookPage_Change(false);
	else if (!State->IsStunState())
		Combat_component->R_Attack();
}


void ACPlayer::R_AttackRelease()
{
	CheckFalse(R_But);
	if (!State->IsInteractionState())
		Combat_component->R_AttackRelease();
	R_But = false;
}


void ACPlayer::Absolut_Attack()
{
	CheckFalse(State->IsIdleState()); //Idel 이 아니라면 안됌

	Combat_component->T_Attack();
}

void ACPlayer::T_AttackRelease()
{
	CheckFalse(State->IsAbsolutState());//궁 사용중이 아니라면 
	Combat_component->T_AttackRelease();
}

//  ==  인벤토리에서 자신 모습 보이게 하는 Actor 추가제거 함수
void ACPlayer::Add_ShowActor(AActor* actor)
{
	if (!!MeshWidget)
		MeshWidget->ShowOnlyActors.Add(actor);
}

void ACPlayer::Remove_ShowActor(AActor* actor)
{
	MeshWidget->RemoveShowOnlyActorComponents(actor);
}


//========================== 건물 지을위치 선정하는 Update ===========================================
void ACPlayer::InteractionBuild_Update()
{
	CheckNull(interact_Build);

	FVector Start = Camera->GetComponentLocation();
	FVector Direction = Camera->GetForwardVector().GetSafeNormal();

	Start += Direction * 50;
	FVector End = Start + Direction * 2500;


	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(interact_Build);

	FHitResult hitResult;
	GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_WorldDynamic, params);
	//DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Red, false, 5.f, 0, 0.1f);

	CheckFalse(hitResult.bBlockingHit);
	if (!!interact_Build)
	{
		CheckFalse(interact_Build->State() == EBuildState::Before);

		interact_Build->SetActorLocation(hitResult.Location);

	}

}
/* Class CameraManger
	InitPlayer 함수에서 초기화한 Manager 를 이용해 보여지는 카메라를 설정  Onwer 를 초기화 함.
*/


class AGameActor* ACPlayer::Get_TargetActor()
{
	return Target_Comp->Get_TargetActor(); 
}

bool ACPlayer::Target_Dot()
{
	return Target_Comp->Target_Dot();
}

void ACPlayer::Camera_Setting(ECamera_Sight input)
{
	sight = input;
	if (sight == ECamera_Sight::FPS) 
	{
		Set_View_Owner_Camera();
		
	}
	else if (sight == ECamera_Sight::TPS) {
		Set_View_Fallow_Camera();
		
	}
}
void ACPlayer::Pressed_Alt()
{

}

void ACPlayer::Released_Alt()
{

}

void ACPlayer::Play_Action(EKeybord lastkey, EKeybord EventKey)
{
	CheckFalse(State->IsIdleState());
	State->Set_ActionMode();
	if (!Target_Comp->Is_TargetMode())
		lastkey = EKeybord::Forward;
	if (EventKey == EKeybord::Shift) 
	{
		switch (lastkey)
		{
			case EKeybord::Forward:
				Combat_component->Play_Action(EActionBut::Shift_F);
				break;

			case EKeybord::Back:
				Combat_component->Play_Action(EActionBut::Shift_B);
				break;

			case EKeybord::Left:
				Combat_component->Play_Action(EActionBut::Shift_L);
				break;

			case EKeybord::Right:
				Combat_component->Play_Action(EActionBut::Shift_R);
				break;

		}
	}
	else if(EventKey == EKeybord::SpaceBar)
	{
		if (!bFight_Mode) 
		{
			FVector vel = GetVelocity();
			Combat_component->Play_Action(EActionBut::Jump_Key);
			Jump();
			UKismetSystemLibrary::K2_SetTimer(this, "Jump_End", 0.1f, true); //땅에 닿을때까지 체크
		}
		else
		{
			switch (lastkey)
			{
			case EKeybord::Forward:
				Combat_component->Play_Action(EActionBut::SpaceBar_F);
				break;

			case EKeybord::Back:
				Combat_component->Play_Action(EActionBut::SpaceBar_B);
				break;

			case EKeybord::Left:
				Combat_component->Play_Action(EActionBut::SpaceBar_L);
				break;

			case EKeybord::Right:
				Combat_component->Play_Action(EActionBut::SpaceBar_R);
				break;

			}
		}
	}
	
}

void ACPlayer::Jump_End()
{
	
	
	CheckTrue(GetCharacterMovement()->IsFalling()); // 날고있으면 return
	
	Combat_component->Jump_End();
	State->Set_IdleMode();
	
	UKismetSystemLibrary::K2_ClearTimer(this, "Jump_End");
}

void ACPlayer::Set_View_Fallow_Camera()
{

	FP_Mesh->SetOwnerNoSee(true);
	GetMesh()->SetOwnerNoSee(false);
	Combat_component->TPS_Weapon();
	sight = ECamera_Sight::TPS;
	GetCharacterMovement()->bOrientRotationToMovement = false; // 뒤로 이동할떄 카메라 흔들림이 수정됌

	SpringArm->SetRelativeLocation(FVector(0, 50, 60));// before (0,50,30)
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;


	Camera->SetRelativeLocation(FVector(0, 0, 0));// before (0,0,30)
	Camera->SetAutoActivate(true);

}
void ACPlayer::Set_Fallow_Camera_Rotation(FRotator rot)
{
	Camera_Manager->Set_Fallow_Camera_Rotation(rot);
}
void ACPlayer::Set_View_FPS_Camera()
{
	SpringArm->SetRelativeLocation(FVector(0, 0, 0));
	Camera->SetRelativeLocation(FVector(30, 0, 70));// before (0,0,30)
	SpringArm->TargetArmLength = 0.f;

	Combat_component->FPS_Weapon();
	sight = ECamera_Sight::FPS;
	APlayerController* control = Cast<APlayerController>(GetController());
	if (!!control) 
	{
		control->SetViewTarget(Camera->GetOwner());
	}
	GetMesh()->SetOwnerNoSee(true);
	FP_Mesh->SetOwnerNoSee(false);
	
	
	
}
void ACPlayer::Set_View_Owner_Camera()
{
	if (!!Camera_Manager) 
	{
		Camera_Manager->Set_View_Owner_Camera();
		GetMesh()->bOwnerNoSee = false;
		FP_Mesh->bOwnerNoSee = true;
	}
	else
		CLog::Log("camera_manager is null");
}
void ACPlayer::Set_View_Dead_Camera()
{
	if (!!Camera_Manager)
		Camera_Manager->Set_View_Dead_Camera();
	else
		CLog::Log("camera_manager is null");
}

void ACPlayer::Set_View_Skill_Camera()
{
	if (!!Camera_Manager)
		Camera_Manager->Set_View_Skill_Camera();
	else
		CLog::Log("camera_manager is null");
}
