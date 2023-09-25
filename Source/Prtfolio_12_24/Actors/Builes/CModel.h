#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CModel.generated.h"

UINTERFACE(MinimalAPI)
class UCModel : public UInterface
{
	GENERATED_BODY()
};
class PRTFOLIO_12_24_API ICModel
{
	GENERATED_BODY()

public:
	virtual class USkeletalMesh* Get_Mesh() = 0;
};
