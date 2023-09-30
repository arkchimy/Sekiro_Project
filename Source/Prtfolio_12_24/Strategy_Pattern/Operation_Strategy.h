#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Actors/GameActor.h"

#include "DataTable/FWeaponData.h"
#include "DataTable/FWeaponData_Enemy.h"
#include "Utilities/CEnums.h"
#include "Utilities/CStructs.h"

#include "Operation_Strategy.generated.h"

UINTERFACE(MinimalAPI)
class UOperation_Strategy : public UInterface
{
	GENERATED_BODY()
};

class PRTFOLIO_12_24_API IOperation_Strategy
{
	GENERATED_BODY()
public:
	
	
public:
	virtual FVector Move(FVector Self_Loc) = 0; // Return Target_Pos
	virtual void Action(class UDataTable* table, class AGameActor* OwnerActor, EMouseBut direction) = 0; // Owner & Target Dot, Cross Product 
	
};
