#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CBuild.generated.h"


UENUM(BlueprintType)
enum EBuildState  // Player에서 CBuild  함수를 사용하기위해 헤더를 추가해서 남겨둠
{
	Before,After,Ready,Max, // After 완료
};

USTRUCT(BlueprintType)
struct FParticle
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UParticleSystem* particle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName Tag;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USoundCue* Particle_Sound;
};

UCLASS()
class PRTFOLIO_12_24_API ACBuild : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray <FParticle> effects;
	bool bParticle_chk = false;
public:	
	ACBuild();
	EBuildState State() { return state; }
	void State(EBuildState val) { state = val; }
	
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:

	UFUNCTION(BlueprintCallable)
		void Particle_Call();
	UFUNCTION(BlueprintCallable)
		void Complete_Location();
	UFUNCTION(BlueprintCallable)
		void Create_Build();// 재료 확인 
	UFUNCTION(BlueprintCallable)
		void Overlap_Event(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	FORCEINLINE bool IsPossible() { return bpossible; }
	FORCEINLINE void Set_Onwer(class ACPlayer* Player) { OwnerCharacter = Player;}

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UStaticMesh* Build_mesh;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UStaticMeshComponent* mesh;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UStaticMeshComponent* Create_mesh;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UParticleSystemComponent* Particle_Comp;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UBoxComponent* Trigger_Mesh;
private:	
	virtual void Tick(float DeltaTime) override;
	
	
	class UMaterialInstanceDynamic* possibleMat;
	class UMaterialInstanceDynamic* ImpossibleMat;
	class UMaterialInstanceDynamic* ReadyMat;

	class UMaterialInterface* CreateMat;
	EBuildState state = EBuildState:: Before;
	bool bpossible = true;

	TArray<class AActor*> OverlapActors;
	class ACPlayer* OwnerCharacter; // 주인 등록
	bool bhasOwner = false;
private:				//속성 관련
	FName Paticle_Tag;//  Fire면 불 상태.
	
};
