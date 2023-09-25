#include "Actors/CWeaponment.h"

#include "Actors/GameActor.h"
#include "Actors/CPlayer.h"
#include "Actors/CEnemy.h"
#include "Actors/CArmor.h"


#include "Global.h"

#include "Particles/ParticleSystemComponent.h"

#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "Sound/SoundCue.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "ActorComponents/State_Component/CStateComponent.h"
#include "ActorComponents/CStatusComponent.h"

ACWeaponment* ACWeaponment::Factory_Weapon(UWorld* InWorld, TSubclassOf<ACWeaponment> WeaponmentClass)
{	// 미사용 중 추후에 팩토리 패턴 연습해보기 
	FActorSpawnParameters param;
	return InWorld->SpawnActor<ACWeaponment>(WeaponmentClass, param);
}

ACWeaponment::ACWeaponment()
{

	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Root_Component");
	RootComponent = Scene;
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Weapon, "WeaponMesh", RootComponent);
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &FP_Weapon, "FPMesh", RootComponent);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &WeaponStatic, "WeaponStatic", Weapon);

	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Block_EffectComp, "Steel_Effect", FP_Weapon);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Wood_EffectComp, "Wood_Effect", FP_Weapon);
	
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Niagara, "Weapon_Niagara", FP_Weapon);

	Block_EffectComp->bAutoActivate = false;
	Wood_EffectComp->bAutoActivate = false;
	
}



void ACWeaponment::BeginPlay()
{

	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner()); //?
	GameActor = Cast<AGameActor>(OwnerCharacter);
	if (!!GameActor) 
	{	// 장착 하였을때 
		Weapon->SetSimulatePhysics(false);
		WeaponStatic->SetSimulatePhysics(false);
		bOnFloor = false;
	}
	else
	{	// inven 에서 버렸을때
		Weapon->SetSimulatePhysics(true);
		WeaponStatic->SetSimulatePhysics(true);
		bOnFloor = true;
		return;
	}
	
	AttachTo(Holster); // 초기에 있을 소켓
	
	if (!!OwnerCharacter) 
	{
		State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
		Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	}
	else
	{
		CLog::Log("OwnerCharacter is Null");
	}

	GetComponents<UShapeComponent>(ShapeComponent);
	for (UShapeComponent* Component : ShapeComponent) // BoxCollider 로 겹침 판단하기. 
	{
		Box_Attach(Component);
		
		Component->OnComponentBeginOverlap.AddDynamic(this, &ACWeaponment::OnComponentBeginOverlap);
		Component->OnComponentEndOverlap.AddDynamic(this, &ACWeaponment::OnComponentEndOverlap);
		
	}
	
	Weapon->SetCastShadow(false);
	Weapon->OnComponentHit.AddDynamic(this, &ACWeaponment::OnComponentHit_Attachmnet);

	if (IsPlayer(OwnerCharacter))
	{
		FP_Weapon->OnComponentHit.AddDynamic(this, &ACWeaponment::OnComponentHit_Attachmnet);
		FP_Weapon->SetCastShadow(false);
		
	}
	
	for(FHas_Armor armor : has_Armor) // 아머 장착한다면.
	{
		FTransform trans;
		ACArmor* temp;
		temp = GetWorld()->SpawnActorDeferred<ACArmor>(armor.Armors, trans, OwnerCharacter, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		armor_Ref.Add(temp);
		AttachTo_Delegate.AddDynamic(temp, &ACArmor::Equip_Armor);
		UGameplayStatics::FinishSpawningActor(temp, trans);
		temp->Equip_Armor();
		temp->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), armor.Socket_Name);
		if (!!player)
			player->Add_ShowActor(temp);

	}	
	
	Off_Collision();

	
	//Niagara->SetVisibility(false);
}

void ACWeaponment::Armor_Destroy()
{
	for(ACArmor* armor : armor_Ref)
	{
		armor->Destroy();
	}
}

//CLog::Print("Overlap Check");


void ACWeaponment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OtherActor->GetClass() == OwnerCharacter->GetClass());// 같으면? 
	CheckTrue(OwnerCharacter == OtherComp->GetOwner());
	CheckTrue(OverlappedComponent->GetOwner() == OtherComp->GetOwner());


	//CLog::Print("Overlap_Chk");
	AGameActor* OwnerActor = Cast<AGameActor>(OwnerCharacter);

	for (AActor* overlapActor : Overlap_Array)
	{
		CheckTrue(OtherActor == overlapActor);
		CheckTrue(OtherComp->GetOwner() == overlapActor);
	}

	FVector location = OverlappedComponent->GetComponentLocation();
	//if (OwnerActor->Target_Dot())
	//{	// 뒤를 잡았다면

	//	AGameActor* HittedActor = OwnerActor->Get_TargetActor();
	//	EDirection Direction = EDirection::Back;
	//	if (!!OwnerActor)
	//		OwnerActor->ParryingMontage(Direction);
	//	else
	//		CLog::Print("OwnerActor Cast fail");
	//	if (!!HittedActor)
	//	{
	//		HittedActor->Set_TargetActor(OwnerActor);
	//		ACharacter* Action_Actor = Cast<ACharacter>(HittedActor);
	//		HittedActor->BeParriedMontage(Direction, Action_Actor);
	//	}
	//	else
	//		CLog::Print("Hitted Cast fail");
	//	return;
	//}
	if(bParrying && OtherComp->ComponentHasTag(FName("Parrying")))
	{	
		// 패링 컴포일경우 데미지 안줌
		// Parrying Success 
		
		AGameActor* HittedActor = Cast<AGameActor>(OtherComp->GetOwner());

		CheckFalse(OwnerActor->Get_ParryingDirection() == HittedActor->Get_BeParriedDirection());
		CheckTrue(OwnerActor->Get_ParryingDirection() == EDirection::Max); // Max는 미등록
		//WeekNotify 의 Direction과 Parring Notify의 Direction이 일치하는 경우
		//EDirection Direction = OwnerActor->TargetDirection(HittedActor);
		EDirection Direction = OwnerActor->Get_ParryingDirection();
		if (!!OwnerActor)
		{
			OwnerActor->ParryingMontage(Direction);
			
		}
		else
			CLog::Print("OwnerActor Cast fail");
		if (!!HittedActor)
		{
			ACharacter* Action_Actor = Cast<ACharacter>(HittedActor);
			HittedActor->BeParriedMontage(Direction, Action_Actor);
			
		}
		else
			CLog::Print("Hitted Cast fail");
		
		
		return;
	}
	

	Overlap_Array.AddUnique(OtherActor);

	ACArmor* Armor = Cast<ACArmor>(OtherComp->GetOwner());

	if (!!Armor) // 부딪힌 Component 가 Armor 라면
	{
		if (Armor->GetOwner() != OwnerCharacter)
		{
			Armor->Armor_TakeDamage(Brake_Power, OwnerCharacter->GetName());
			Overlap_Array.AddUnique(OtherComp->GetOwner()->GetOwner());
			
		}

	}
	bool blocking_chk = false;
	for( FName tag_data : OtherComp->ComponentTags)
	{
		for (FName tag : Block_Tag) 
		{
			if (tag_data == tag)
			{
				blocking_chk = true;
				break;
			}
		}
		if (blocking_chk)
			break;
	}
	
	

		

	if (blocking_chk) // 캐릭터가 아니라면 Attach_Effect
	{
		
		if (!!Armor) // 부딪힌 Component 가 Armor 라면
		{
			if (Armor->GetOwner() != OwnerCharacter)
			{
				Armor->Armor_TakeDamage(Brake_Power, OwnerCharacter->GetName());
				CLog::Log("Armor_TakeDamage");
			}
		}

		FVector hit_location = Hit.Location;
		FRotator rotator = FP_Weapon->GetRelativeRotation();
		FTransform trans;
		trans.SetLocation(location);
		trans.SetRotation(FQuat(rotator));
		trans.SetScale3D(FVector(Block_EffectComp->GetComponentScale()));

		if (OtherComp->ComponentHasTag("Weapon")) // Sound 체크만
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordBlock_SoundCue, location);
		else if (OtherComp->ComponentHasTag("Steel"))
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SteelBlock_SoundCue, location);

		
		if (OtherComp->ComponentHasTag("Wood"))   //재질에 따른 이팩트
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), WoodBlock_SoundCue, location);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Wood_EffectComp->Template, trans, true);
			//Attach 된 Armor 만 
			
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Block_EffectComp->Template, trans, true);
		}
		
	}
	else if (TakeDamage_Delegate.IsBound()) // 막히지않았으면 데미지 측정
	{
		if (!!Hit_SoundCue)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Hit_SoundCue, location);
		TakeDamage_Delegate.Broadcast(Hit, OverlappedComponent->GetComponentLocation(), OverlappedComponent->GetOwner(),AttackTag_Type,Knoback_Power);
		
	}

	

}

void ACWeaponment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (On_Weaponment_EndOverlap.IsBound())
		On_Weaponment_EndOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
}

void ACWeaponment::OnComponentHit_Attachmnet(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	

	
}



void ACWeaponment::AttachToCollision(UShapeComponent* collision ,FName Socket_name)
{
	
	
	player = Cast<ACPlayer>(OwnerCharacter);
	enemy = Cast<ACEnemy>(OwnerCharacter);
	ShapeComponent.Add(collision);

	if (!!enemy) 
	{
		collision->AttachToComponent(enemy->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Socket_name);
		
	}
	else if (!!player) 
	{
		collision->AttachToComponent(player->Get_FP_Mesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), Socket_name);
		
	}
}

void ACWeaponment::AttachTo(FName Socket_name)
{
	

	player = Cast<ACPlayer>(OwnerCharacter);
	enemy = Cast<ACEnemy>(OwnerCharacter);
	attachSocket = Socket_name;
	CheckTrue(OwnerCharacter != this->GetOwner());
	if (!!player) 
	{
		Weapon->AttachToComponent(player->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), attachSocket);
		FP_Weapon->AttachToComponent(player->Get_FP_Mesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), attachSocket);
		player->Add_ShowActor(this);
	}
	else if (!!enemy) 
	{
		FP_Weapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), attachSocket);
		Weapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), attachSocket);
	}

	//FP_Weapon->SetOwnerNoSee(true);
	//Weapon->SetOwnerNoSee(false);
	FP_Weapon->SetOwnerNoSee(true);
}


void ACWeaponment::FPS_Weapon()
{
	//CLog::Print(this->GetName());
	FP_Weapon->SetOwnerNoSee(false);
	Weapon->SetOwnerNoSee(true);
}

void ACWeaponment::TPS_Weapon()
{
	FP_Weapon->SetOwnerNoSee(true);
	Weapon->SetOwnerNoSee(false);
	
	FP_Weapon->SetVisibility(false);
	Weapon->SetVisibility(true);

}

void ACWeaponment::AttachToloop()
{
	AttachTo(attachSocket);
	
}

void ACWeaponment::Weapon_Niagara(bool val)
{
	Niagara->SetVisibility(val);
}

void ACWeaponment::On_Collision(EAttackTag_Type AttackType, float Knoback)
{
	
	Overlap_Array.Empty();
	AttackTag_Type = AttackType;
	Knoback_Power = Knoback;

	ShapeComponent.Empty();
	GetComponents<UShapeComponent>(ShapeComponent);
	for (UShapeComponent* component : ShapeComponent)
	{
		component->SetVisibility(true);
		component->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
		component->SetGenerateOverlapEvents(true);
	}
	Weapon->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3); // Channel3  WeaponDynamic
	Weapon->SetGenerateOverlapEvents(true);
	if(!!WeaponStatic)
	{
		WeaponStatic->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
		WeaponStatic->SetGenerateOverlapEvents(true);
	}
}

void ACWeaponment::Off_Collision()
{

	
	for(UShapeComponent* component : ShapeComponent)
	{
		component->SetVisibility(false);
		component->SetGenerateOverlapEvents(false);
		component->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);

	}
	Weapon->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4); // Channel3  WeaponDynamic
	FP_Weapon->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
	if (!!WeaponStatic)
	{
		WeaponStatic->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
		WeaponStatic->SetGenerateOverlapEvents(false);
	}
}

void ACWeaponment::On_Parrying()
{
	Overlap_Array.Empty();
	bParrying = true;
	for (UShapeComponent* component : ShapeComponent)
	{
		component->SetVisibility(false);
		component->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel13);// Channel 13 is  ParringMode
		component->SetGenerateOverlapEvents(true);
	}
	Weapon->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel13); // Channel3  WeaponDynamic
	Weapon->SetGenerateOverlapEvents(true);
	if (!!WeaponStatic)
	{
		WeaponStatic->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel13);
	}
}

void ACWeaponment::Off_Parrying()
{
	bParrying = false;
	for (UShapeComponent* component : ShapeComponent)
	{
		component->SetVisibility(false);
		component->SetGenerateOverlapEvents(false);
		component->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
	}
	Weapon->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4); // Channel3  WeaponDynamic
	if (!!WeaponStatic)
	{
		WeaponStatic->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
	}
}

void ACWeaponment::Item_PickUp()
{
	Destroy();
}

void ACWeaponment::Destroy_Armor()
{
	for(ACArmor* armor : armor_Ref)
	{
		if(!!player)
		{
			player->Remove_ShowActor(armor);
		}
		armor->Destroy();
	}
}

void ACWeaponment::Set_CollisionTag(TArray<ECollisionTags> collsiontags)
{
	collison_tags = collsiontags;
}

bool ACWeaponment::IsPlayer(ACharacter* InOwnerCharacter)
{
	player = Cast<ACPlayer>(InOwnerCharacter);
	if (!!player)
		return true;
	return false;
}

bool ACWeaponment::IsFloorItem()
{
	
	return WeaponStatic->IsAnySimulatingPhysics();
}



