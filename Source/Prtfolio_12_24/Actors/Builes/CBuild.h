#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CBuild.generated.h"


UENUM(BlueprintType)
enum EBuildState  // Player���� CBuild  �Լ��� ����ϱ����� ����� �߰��ؼ� ���ܵ�
{
	Before,After,Ready,Max, // After �Ϸ�
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
		void Create_Build();// ��� Ȯ�� 
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
	class ACPlayer* OwnerCharacter; // ���� ���
	bool bhasOwner = false;
private:				//�Ӽ� ����
	FName Paticle_Tag;//  Fire�� �� ����.
	
};
