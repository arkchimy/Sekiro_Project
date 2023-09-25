#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/Builes/CBuild.h"
#include "CThrow_AttachDummy.generated.h"

UCLASS()
class PRTFOLIO_12_24_API ACThrow_AttachDummy : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UParticleSystemComponent* Particle_Comp;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<FParticle> particle;
public:	
	ACThrow_AttachDummy();

protected:
	virtual void BeginPlay() override;

public:	
	void Set_StaticMesh(class UStaticMesh* mesh);
	void Set_Visible(bool inval);
	void Set_Owner(class ACharacter* InCharacter);
	FORCEINLINE FName Get_State() { return State; }
public:
	void Trigger_TagChk(FName val);
	class ACharacter* OwnerCharacter;
private:
	FName State;
};
