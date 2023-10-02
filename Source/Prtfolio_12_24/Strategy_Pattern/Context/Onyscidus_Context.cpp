#include "Strategy_Pattern/Context/Onyscidus_Context.h"
#include "Global.h"

UOnyscidus_Context::UOnyscidus_Context()
	: UOperation_Context()
{
	CHelpers::GetAsset<UDataTable>(&Montage_Table, L"DataTable'/Game/_Character/Enemy/Onyscidus/BP_Onyscidus_Combo.BP_Onyscidus_Combo'");
	
}

void UOnyscidus_Context::Action(FVector Target_Location, AGameActor* Target)
{
	UOperation_Context::Action(Target_Location, Target);
}