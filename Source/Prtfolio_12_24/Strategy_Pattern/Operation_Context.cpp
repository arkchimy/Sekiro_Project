#include "Operation_Context.h"


#include "Actors/GameActor.h"
#include "Global.h"

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
void UOperation_Context::Action(AGameActor* Target)
{
	CheckNull(Montage_Table);
	CheckNull(OwnerActor);
	EMouseBut direction = EMouseBut::Front;
	if(!!Target)
		Hiteed_Direction(Target, &direction);
	Operation->Action(Montage_Table, OwnerActor, direction);
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