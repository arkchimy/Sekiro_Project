#pragma once

#include "CoreMinimal.h"
#include "Actors/Enemies/CAIController_Enemy.h"
#include "MyCAIController_Archer.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API AMyCAIController_Archer : public ACAIController_Enemy
{
	GENERATED_BODY()
public:
	AMyCAIController_Archer();
public:
	virtual void BeginPlay() override;
private:
	FString dasdasdasd = "Archer";
};
