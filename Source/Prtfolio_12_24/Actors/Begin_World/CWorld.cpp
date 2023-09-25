#include "Actors/Begin_World/CWorld.h"
#include "Global.h"

ACWorld::ACWorld()
{
	
}

ACModelFactory* ACWorld::Get_ModelFactory()
{
	if (!!ModelFactory)
		return ModelFactory;
	ModelFactory = ACModelFactory::SpawnFactory(GetWorld());
	return ModelFactory;
}

void ACWorld::BeginPlay()
{
	Super::BeginPlay();
	
}

