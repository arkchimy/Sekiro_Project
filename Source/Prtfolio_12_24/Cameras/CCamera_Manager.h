#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCamera_Manager.generated.h"

UCLASS()
class PRTFOLIO_12_24_API ACCamera_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	ACCamera_Manager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class ACFallow_Camera> Fallow_Camera_class;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class ACDead_Camera> Dead_Camera_class;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<TSubclassOf<class ACDead_Camera>> Skill_class;
public:
	
	void Set_View_Fallow_Camera();
	void Set_View_Owner_Camera();
	void Set_View_Dead_Camera();
	void Set_View_Skill_Camera();

	FRotator Get_Fallow_Camera_Rotation();
	void Set_Fallow_Camera_Rotation(FRotator rot);
	FORCEINLINE void Set_Owner_Camera(class UCameraComponent* InOwnerCamera) {OwnerCamera = InOwnerCamera;}
	void DrawLine_Forward();
private:
	
	class ACharacter* OwnerCharacter;
	class ACPlayer* player;

	class ACDead_Camera* Dead_Camera;
	class ACFallow_Camera* Fallow_Camera;
	class ACDead_Camera* Skill_Camera;

	class UCameraComponent* OwnerCamera;
	
};
