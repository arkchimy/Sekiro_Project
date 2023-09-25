#include "CModelFactory.h"
#include "Global.h"
#include "CPageModel.h"
#include "CModel.h"


ACModelFactory::ACModelFactory()
{
	
}


ICModel* ACModelFactory::GetModel(EModelType type)
{
	switch(type)
	{
	case EModelType::Page : 
		if (!!pageModel) {
			
			
			return Cast<ICModel>(pageModel);
		}
		pageModel = NewObject<ACPageModel>();
		ICModel* model = Cast<ICModel>(pageModel);
		return model;
	}
	return nullptr;
}

ACModelFactory* ACModelFactory::SpawnFactory(UWorld* InWorld)
{
	FActorSpawnParameters param;
	
	return InWorld->SpawnActor<ACModelFactory>(param);
}

void ACModelFactory::BeginPlay()
{
	Super::BeginPlay();
}


//================================½Ì±ÛÅæ ================================
//ACModelFactory* ACModelFactory::instance = nullptr;
//
//ACModelFactory::ACModelFactory()
//{
//	if (instance == nullptr)
//		instance = this;
//}
//
//
//ACModelFactory* ACModelFactory::Spawn(UWorld* InWorld)
//{
//
//	FActorSpawnParameters params;
//	instance = InWorld->SpawnActor<ACModelFactory>(params);
//	return instance;
//}
//ACPageModel* ACModelFactory::PageModel()
//{
//	return pageModel;
//}
//
//
//void ACModelFactory::BeginPlay()
//{
//	Super::BeginPlay();
//	pageModel = ACPageModel::Spawn(GetWorld());
//
//}
