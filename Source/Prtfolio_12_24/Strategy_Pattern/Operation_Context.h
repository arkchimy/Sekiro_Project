#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Strategy_Pattern/Operation_Strategy.h"
#include "Strategy_Pattern/Patrol_Strategy.h"
#include "Engine/DataTable.h"

#include "Utilities/CStructs.h"
#include "Utilities/CEnums.h"

#include "Operation_Context.generated.h"

UCLASS()
class PRTFOLIO_12_24_API UOperation_Context : public UObject
{
	GENERATED_BODY()

public:
	void Set_Operation(class IOperation_Strategy* input);

	virtual void Move(FVector Self_Loc);
	virtual void Action(FVector Target_Location,class AGameActor* Target = nullptr);
	void Hiteed_Direction(AActor* Causer, EMouseBut* direction);	// Target != nullptr;
	void Hiteed_Direction(FVector Loc, EMouseBut* direction);	    // Target ==  nullptr;

	void Get_coordinate(const FRotator Rot,Ecoordinate* coor);
	EMouseBut FindLookAtTarget(Ecoordinate* Owner, Ecoordinate* Target);
	void Init_Context(class AGameActor* Owner);

private:
	class IOperation_Strategy* Operation;
	class AGameActor* OwnerActor;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UDataTable* Montage_Table; //  상속시켜서 각자 초기화
	FVector Target_Location = FVector(0, 0, 0);
};