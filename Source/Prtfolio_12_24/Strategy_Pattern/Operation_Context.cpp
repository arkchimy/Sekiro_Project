#include "Operation_Context.h"

#include "ActorComponents/State_Component/CStateComponent.h"
#include "Actors/GameActor.h"
#include "Global.h"

void UOperation_Context::Get_coordinate(const FRotator Rot , Ecoordinate* coor)
{
	if (0 < Rot.Yaw)
	{	// 1사분면 or 4사분면
		*coor = 90 < Rot.Yaw ? Ecoordinate::RD : Ecoordinate::RU;
		return;
	}
	*coor =  -90 < Rot.Yaw  ? Ecoordinate::LU : Ecoordinate::LD;
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
void UOperation_Context::Action(FVector input_data, AGameActor* Target)
{
	CheckNull(Montage_Table);
	CheckNull(OwnerActor);
	UCStateComponent* State = CHelpers::GetComponent<UCStateComponent>(OwnerActor);
	CheckNull(State);

	if (State->IsInteractionState())
	{
		Target_Location = Operation->Move(OwnerActor->GetActorLocation(), Target);// 자신의 위치를 넘겨주고 Target_Location을 정의
		Move(Target_Location);
		return;
	}
	CheckTrue(State->IsActionState());
	State->Set_ActionMode();
	
	
	EMouseBut direction = EMouseBut::Front;
	if (!!Target) 
	{
		Hiteed_Direction(Target, &direction);
	}
	else
		Hiteed_Direction(Target_Location, &direction);
	if (!!Operation) 
	{
		Operation->Action(Target_Location, Montage_Table, OwnerActor, direction);
	}
	else
		CLog::Print("Operation is Null");

}
void UOperation_Context::Move(FVector Target_Loc)
{	// Defalut == Home_Location  || Target_Location  
	CLog::Print(Target_Loc);
	FRotator Owner_Rot = OwnerActor->GetActorRotation();
	FRotator Target_Rot = UKismetMathLibrary::FindLookAtRotation(OwnerActor->GetActorLocation(), Target_Loc);
	
	FRotator rot = UKismetMathLibrary::RInterpTo(Owner_Rot, Target_Rot, 0.1f, 5.f);

	OwnerActor->SetActorRotation(FRotator(Owner_Rot.Pitch, rot.Yaw, Owner_Rot.Roll));
}

void UOperation_Context::Hiteed_Direction(AActor* Causer, EMouseBut* direction)
{
	
	CheckNull(OwnerActor);
	FRotator Owner_Rot = OwnerActor->GetActorRotation();

	FRotator Target_Rot = UKismetMathLibrary::FindLookAtRotation(OwnerActor->GetActorLocation(), Causer->GetActorLocation());
	Ecoordinate Owner_Coor;
	Ecoordinate Target_Coor;
	Get_coordinate(Owner_Rot, &Owner_Coor);
	Get_coordinate(Target_Rot, &Target_Coor);
	*direction = FindLookAtTarget(&Owner_Coor, &Target_Coor);

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
