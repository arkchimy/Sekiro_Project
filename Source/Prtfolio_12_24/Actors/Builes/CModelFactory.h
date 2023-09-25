#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Model_Factory.h"
#include "CModelFactory.generated.h"



UCLASS()
class PRTFOLIO_12_24_API ACModelFactory : public AActor , public IModel_Factory
{
	GENERATED_BODY()

private:
	ACModelFactory();
	
public:
	virtual class ICModel* GetModel(EModelType type) override;
	static ACModelFactory* SpawnFactory(UWorld* InWorld);
protected:
	virtual void BeginPlay() override;

	

private:
	class ACPageModel* pageModel;
	int cnt = 0;

};



//ΩÃ±€≈Ê
//UCLASS()
//class PRTFOLIO_12_24_API ACModelFactory : public AActor, public IModel_Factory
//{
//	GENERATED_BODY()
//
//private:
//	ACModelFactory();
//	static ACModelFactory* instance;
//public:
//	static ACModelFactory* Spawn(class UWorld* InWorld);
//	class ACPageModel* PageModel();
//
//protected:
//	virtual void BeginPlay() override;
//
//
//
//private:
//	class ACPageModel* pageModel;
//
//
//};
