#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/Builes/CModelFactory.h"
#include "CWorld.generated.h"

UCLASS()
class PRTFOLIO_12_24_API ACWorld : public AActor
{
	GENERATED_BODY()
	
public:	
	ACWorld();
public:
	ACModelFactory* Get_ModelFactory();
protected:
	virtual void BeginPlay() override;

private:
	ACModelFactory* ModelFactory;
};
