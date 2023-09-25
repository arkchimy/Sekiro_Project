#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPage_Test.generated.h"


UCLASS()
class PRTFOLIO_12_24_API ACPage_Test : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPage_Test();
	class USkeletalMeshComponent* Mesh;
protected:
	
	virtual void BeginPlay() override;

	
private:
	
};
