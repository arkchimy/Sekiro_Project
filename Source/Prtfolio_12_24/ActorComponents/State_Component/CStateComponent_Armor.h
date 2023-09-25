#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent_Armor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRTFOLIO_12_24_API UCStateComponent_Armor : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float Health_Max = 3.0f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Armor_Diffense")
		float Armor_Power = 0.0f;
public:	
	UCStateComponent_Armor();

protected:
	virtual void BeginPlay() override;

public:	
	void Sub_Health(float inval,FString Attacker);
	void Add_Health(float inval);
	FORCEINLINE bool IsLife() { return Life;  }
protected:
	class ACArmor* Armor; // Armor	
	
	float Health = 1;
	bool Life = true;
};
