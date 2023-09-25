#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CThrow_ManagerActor.generated.h"

UCLASS()
class PRTFOLIO_12_24_API ACThrow_ManagerActor : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<TSubclassOf<class ACThrowItem>> Throwitem_class;

public:	
	ACThrow_ManagerActor();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		class ACThrowItem* Find_ThrowActor(int class_id);
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<ACThrowItem*> Throw_Array;
	ACThrowItem* result_item = nullptr;
};
