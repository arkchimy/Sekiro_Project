#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CModel.h"
#include "CPageModel.generated.h"

UCLASS()
class PRTFOLIO_12_24_API ACPageModel : public AActor , public ICModel
{
	GENERATED_BODY()
	
public:	
	ACPageModel();
	
protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE USkeletalMesh* Get_Mesh() { return Mesh; }
	
private:
	class USkeletalMeshComponent* MeshComp;
	class USkeletalMesh* Mesh;
};
