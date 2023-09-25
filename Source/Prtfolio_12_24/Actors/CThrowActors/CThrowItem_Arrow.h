#pragma once

#include "CoreMinimal.h"
#include "Actors/CThrowActors/CThrowItem.h"
#include "CThrowItem_Arrow.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API ACThrowItem_Arrow : public ACThrowItem
{
	GENERATED_BODY()
public:
	ACThrowItem_Arrow();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		bool bRYP = false;
protected:
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnThrowComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) override;
	
	virtual void Set_Pressed(float inval, ACharacter* OwnerCharacter) override;

	
	UFUNCTION()
		virtual void hit_Posible();
	UFUNCTION()
		virtual void Trail_AutoOff();

	virtual void Draw_Route(FVector Hit_location) override;
	FVector Impulse_Result;
	FVector Start;
	float Broken_Power = 1.0f;
	

	//class ACharacter* OwnerCharacter;
	class ACPlayer* player;

};
