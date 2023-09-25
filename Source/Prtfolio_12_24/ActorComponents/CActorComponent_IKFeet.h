#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActorComponent_IKFeet.generated.h"


USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FVector Left_Location; // X축 담당
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FVector Right_Location; // X축 담당
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FRotator Left_Rotation;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FRotator Right_Rotation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FVector Pervious; //Z축 담당
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRTFOLIO_12_24_API UCActorComponent_IKFeet : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActorComponent_IKFeet();
public:
	
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FFeetData FeetData;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float InterpSpeed = 17.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FName Left_Socket = FName("ik_foot_l");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FName Right_Socket = FName("ik_foot_R");

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
		float TraceDistance = 55;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "InverseKinemetics")
		float OffsetDistance = 5;
protected:
	virtual void BeginPlay() override;


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	void Trace(FName InSocket, float& OutDistance, FRotator& OutRotation);
	void LineTarce(FName Socket ,float &distance, FRotator& rotator);
public:
	FORCEINLINE FFeetData Get_FeetData() { return FeetData; }

	
private:
	class ACPlayer* player;
	class ACharacter* OwnerCharacter;
	float Capsule_Distance = 0.f;
	float Scale;
};
