
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Model_Factory.generated.h"

UENUM(BlueprintType)
enum class EModelType : uint8
{
	Page, Tree, Max,
};

UINTERFACE(MinimalAPI)
class UModel_Factory : public UInterface
{
	GENERATED_BODY()
};


class PRTFOLIO_12_24_API IModel_Factory
{
	GENERATED_BODY()

public:
	virtual class ICModel* GetModel(EModelType type) = 0;
};
