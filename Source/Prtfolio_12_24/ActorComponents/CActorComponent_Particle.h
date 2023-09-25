#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/Builes/CBuild.h"
#include "CActorComponent_Particle.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRTFOLIO_12_24_API UCActorComponent_Particle : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCActorComponent_Particle();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:
	void Attach_Particle(FName tag,float timer);
	UFUNCTION()
		void Auto_ParticleOff();
	UFUNCTION()
		void Attached_Damaged();
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FParticle> possible_Particle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UParticleSystemComponent* comp;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		float Fire_Damage = 1.0f;

private:
	
	class USoundCue* DamagedSound;
	class ACArmor* armor;
	class ACPlayer* player;
	class ACEnemy* enemy;


};
