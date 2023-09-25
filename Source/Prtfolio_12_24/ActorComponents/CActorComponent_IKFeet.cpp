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
	

	float Left_Distance; // Interp 하기위해서 구하는 Goal
	float Right_Distance; //  실시간 목표 지점
	FRotator L_Rotation;  // Interp 현재위치 , 목표위치 ,DeltaTime, speed 
	FRotator R_Rotation;  //  실시간 목표 회전

	LineTarce(Left_Socket, Left_Distance, L_Rotation);
	LineTarce(Right_Socket, Right_Distance, R_Rotation);

	float offset = FMath::Min(Left_Distance, Right_Distance); // root 하위 소켓인 pervious 의 높이를 조절함
	
	FeetData.Pervious.Z = UKismetMathLibrary::FInterpTo(FeetData.Pervious.Z, offset, DeltaTime, InterpSpeed);//몸체의 위치

	FeetData.Left_Location.X = UKismetMathLibrary::FInterpTo(FeetData.Left_Location.X, Left_Distance - offset , DeltaTime, InterpSpeed); // 왼발의 놆이
	FeetData.Right_Location.X = UKismetMathLibrary::FInterpTo(FeetData.Right_Location.X, -(Right_Distance - offset ), DeltaTime, InterpSpeed); // 왼발의 높이 

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
		
		CheckFalse(FHit.IsValidBlockingHit());// 안맞았을 경우 Return
		distance = (FHit.ImpactPoint - End).Size() - TraceDistance; // Z축 구하기
		
		float pitch = -UKismetMathLibrary::DegAtan2(FHit.ImpactNormal.X, FHit.ImpactNormal.Z); // 법선 벡터의 X축과 Z축의 기울기
		float roll = UKismetMathLibrary::DegAtan2(FHit.ImpactNormal.Y, FHit.ImpactNormal.Z); // 법선 벡터의 X축과 Z축의 기울기
		
		
		rotator = FRotator(pitch, 0.0f, roll);
		

	}
}