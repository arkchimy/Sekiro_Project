#include "ActorComponents/CActorComponent_IKFeet.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "Actors/CPlayer.h"
#include "GameFramework/Character.h"


UCActorComponent_IKFeet::UCActorComponent_IKFeet()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}



void UCActorComponent_IKFeet::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	Scale = GetOwner()->GetTransform().GetScale3D().Z;
	if(!!OwnerCharacter)
		Capsule_Distance = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() ;
	
}


void UCActorComponent_IKFeet::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	float Left_Distance; // Interp �ϱ����ؼ� ���ϴ� Goal
	float Right_Distance; //  �ǽð� ��ǥ ����
	FRotator L_Rotation;  // Interp ������ġ , ��ǥ��ġ ,DeltaTime, speed 
	FRotator R_Rotation;  //  �ǽð� ��ǥ ȸ��

	LineTarce(Left_Socket, Left_Distance, L_Rotation);
	LineTarce(Right_Socket, Right_Distance, R_Rotation);

	float offset = FMath::Min(Left_Distance, Right_Distance); // root ���� ������ pervious �� ���̸� ������
	
	FeetData.Pervious.Z = UKismetMathLibrary::FInterpTo(FeetData.Pervious.Z, offset, DeltaTime, InterpSpeed);//��ü�� ��ġ

	FeetData.Left_Location.X = UKismetMathLibrary::FInterpTo(FeetData.Left_Location.X, Left_Distance - offset , DeltaTime, InterpSpeed); // �޹��� ����
	FeetData.Right_Location.X = UKismetMathLibrary::FInterpTo(FeetData.Right_Location.X, -(Right_Distance - offset ), DeltaTime, InterpSpeed); // �޹��� ���� 

	FeetData.Left_Rotation = UKismetMathLibrary::RInterpTo(FeetData.Left_Rotation, L_Rotation, DeltaTime, InterpSpeed);
	FeetData.Right_Rotation = UKismetMathLibrary::RInterpTo(FeetData.Right_Rotation, R_Rotation, DeltaTime, InterpSpeed);
}

void UCActorComponent_IKFeet::LineTarce(FName Socket, float& distance,FRotator& rotator)
{
	if (!!OwnerCharacter)
	{
		distance = 0.0f;
		FVector Socket_Location = OwnerCharacter->GetMesh()->GetSocketLocation(Socket);
		FVector Start = FVector(Socket_Location.X, Socket_Location.Y, OwnerCharacter->GetActorLocation().Z);
		FVector End = FVector(Start.X, Start.Y, (Start.Z - Capsule_Distance - TraceDistance) * Scale);
		
		TArray<AActor*> Ignore_Actor;
		Ignore_Actor.Add(OwnerCharacter);

		FHitResult FHit;
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, Ignore_Actor, EDrawDebugTrace::None, FHit, true, FLinearColor::Green, FLinearColor::Red,0.1f);
		
		CheckFalse(FHit.IsValidBlockingHit());// �ȸ¾��� ��� Return
		distance = (FHit.ImpactPoint - End).Size() - TraceDistance; // Z�� ���ϱ�
		
		float pitch = -UKismetMathLibrary::DegAtan2(FHit.ImpactNormal.X, FHit.ImpactNormal.Z); // ���� ������ X��� Z���� ����
		float roll = UKismetMathLibrary::DegAtan2(FHit.ImpactNormal.Y, FHit.ImpactNormal.Z); // ���� ������ X��� Z���� ����
		
		
		rotator = FRotator(pitch, 0.0f, roll);
		

	}
}