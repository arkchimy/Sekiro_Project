#include "Operation_Context.h"


#include "Actors/GameActor.h"
#include "Global.h"

void UOperation_Context::Get_coordinate(const FRotator Rot , Ecoordinate* coor)
{
	if (0 < Rot.Yaw)
	{	// 1사분면 or 4사분면
		*coor = 90 < Rot.Yaw ? Ecoordinate::RD : Ecoordinate::RU;
		return;
	}
	*coor =  Rot.Yaw < -90 ? Ecoordinate::LU : Ecoordinate::LD;
}

EMouseBut UOperation_Context::FindLookAtTarget(Ecoordinate* Owner, Ecoordinate* Target)
{
	if (*Owner == *Target)	// 같은 사분면이면 Front
		return EMouseBut::Front;
	int result = int32(*Owner) - int32(*Target);
	if (result == 2 || result == -2) // 차이가 2라면 2사분면차이 즉 Back
		return EMouseBut::Back;

	return 0 < result ? EMouseBut::Left : EMouseBut::Right;
}

void UOperation_Context::Init_Context(AGameActor* Owner)
{
	CheckNull(Owner);
	OwnerActor = Owner;
}
void UOperation_Context::Set_Operation(class IOperation_Strategy* input)
{
	CheckNull(input);
	Operation = input;
}
void UOperation_Context::Action(FVector Target_Location, AGameActor* Target)
{
	CheckNull(Montage_Table);
	CheckNull(OwnerActor);
	
	EMouseBut direction = EMouseBut::Front;
	if(!!Target)
		Hiteed_Direction(Target, &direction);
	else
		Hiteed_Direction(Target_Location, &direction);
	if (!!Operation)
		Operation->Action(Target_Location,Montage_Table, OwnerActor, direction);
	else
		CLog::Print("Operation is Null");

	if (direction == EMouseBut::Back)
		CLog::Print("EMouseBut::Back");
	else if (direction == EMouseBut::Left)
		CLog::Print("EMouseBut::Left");
	else if (direction == EMouseBut::Right)
		CLog::Print("EMouseBut::Right");
	else if (direction == EMouseBut::Front)
		CLog::Print("EMouseBut::Front");

}
FVector UOperation_Context::Move(FVector Self_Loc)
{
	return Operation->Move(Self_Loc);
}

void UOperation_Context::Hiteed_Direction(AActor* Causer, EMouseBut* direction)
{
	float dot_Result;
	FVector Cross_Result;
	
	
	
	dot_Result = UKismetMathLibrary::Dot_VectorVector(OwnerActor->GetActorForwardVector(), Causer->GetActorForwardVector());
	Cross_Result = UKismetMathLibrary::Cross_VectorVector(OwnerActor->GetActorForwardVector(), Causer->GetActorForwardVector());
	

	bool Forward_Chk, Right_Chk;

	Forward_Chk = dot_Result < 0 ? true : false; // 정면에서 맞음

	if (-0.5f < Cross_Result.Z && Cross_Result.Z < 0.5f)
	{
		/*if (Forward_Chk)
			CLog::Print("Front");
		else
			CLog::Print("Back");*/
		*direction = Forward_Chk ? EMouseBut::Front : EMouseBut::Back;
	}
	else
	{
		Right_Chk = 0 < Cross_Result.Z ? true : false;
		/*if (Right_Chk)
			CLog::Print("Left");
		else
			CLog::Print("Right");*/
		*direction = Right_Chk ? EMouseBut::Left : EMouseBut::Right;
	}

}
void UOperation_Context::Hiteed_Direction(FVector Target_Loc, EMouseBut* direction)
{
	
	CheckNull(OwnerActor);
	FRotator Owner_Rot = OwnerActor->GetActorRotation();
	FRotator Target_Rot = UKismetMathLibrary::FindLookAtRotation(OwnerActor->GetActorLocation(), Target_Loc);
	Ecoordinate Owner_Coor;
	Ecoordinate Target_Coor;
	Get_coordinate(Owner_Rot,&Owner_Coor);
	Get_coordinate(Target_Rot ,&Target_Coor);
	*direction = FindLookAtTarget(&Owner_Coor,&Target_Coor);
	

}

//void UOperation_Context::Hiteed_Direction(FVector Loc, EMouseBut* direction)
//{
//	// Direction 구한것  제대로 정리하기.
//
//	// 완전히 Montage가 종료하고 실행을 하는지 구현하고 만약 아니라면 Action State 를 이용해서 구현할 방법찾기.
//	CheckNull(OwnerActor);
//	CLog::Print("Loc_Direct Called");
//	FRotator target = UKismetMathLibrary::FindLookAtRotation(OwnerActor->GetActorLocation(), FVector(0, 0, 0));
//	if (target.Yaw < 0) {// 180 Over
//		target.Yaw += 360;
//
//	}
//	FRotator rot = OwnerActor->GetActorRotation() - target;
//	if (OwnerActor->GetActorRotation().Yaw < 0)
//	{
//		rot.Yaw += 360;
//
//	}
//	bool R_chk = rot.Yaw < 0;
//	rot.Yaw = rot.Yaw <= 0 ? -rot.Yaw : rot.Yaw;
//	FString result;
//	if (180 < rot.Yaw)
//	{
//		rot.Yaw -= (int(rot.Yaw) % 180) * 2;
//	}
//	int Front_Start = 0, Front_End = 45;
//	int Right_Start = 45, Right_End = 135;
//	int Back_Start = 135, Back_End = 180;
//
//	if (Front_Start <= rot.Yaw && rot.Yaw < Front_End)
//		*direction = EMouseBut::Front;
//	else if ((Right_Start <= rot.Yaw && rot.Yaw < Right_End))
//	{
//		if (R_chk)
//			*direction = EMouseBut::Right;
//		else
//			*direction = EMouseBut::Left;
//	}
//	else
//		*direction = EMouseBut::Back;
//	/*else if ((Back_Start <= rot.Yaw && rot.Yaw < Back_End))
//		*direction = EMouseBut::Back;*/
//
//	CLog::Print(rot.Yaw);
//	CLog::Print(result);
//}