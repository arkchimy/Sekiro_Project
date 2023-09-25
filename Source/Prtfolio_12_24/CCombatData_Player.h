#pragma once

#include "CoreMinimal.h"

#include "Utilities/CEnums.h"
#include "Utilities/CStructs.h"
#include "Engine/DataTable.h"

#include "CCombatData.h" //  need
#include "CCombatData_Player.generated.h"

UCLASS()
class PRTFOLIO_12_24_API UCCombatData_Player : public UCCombatData
{
	GENERATED_BODY()
public:
	virtual void BeginPlay(class ACharacter* InOwnerCharacter) override;

private:
	FPlayer_Action Action;
	
	class ACPlayer* player;
public:
	FORCEINLINE virtual ECamera_Sight Get_Sight() override { return Sight; }
	virtual void Play_Action( const EActionBut but) override ;

	virtual void Jump_End() override;
};
