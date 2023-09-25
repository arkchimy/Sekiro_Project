#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CFallow_Camera.generated.h"

UCLASS()
class PRTFOLIO_12_24_API ACFallow_Camera : public AActor
{
	GENERATED_BODY()
	
public:	
	ACFallow_Camera();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USceneComponent* Root;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USpringArmComponent* Spring;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCameraComponent* Camera;


public:
	void Set_OwnerCharacter(class ACharacter* inOwner);
	void Set_View_Fallow_Camera();
	
	void DrawLine_Forward();


	FRotator Get_Fallow_Camera_Rotation();
	void Set_Fallow_Camera_Rotation(FRotator rot); // Targeting 을 할떄 변환
	UFUNCTION()
		void Camera_HRotation(float Axis);
	UFUNCTION()
		void Camera_VRotation(float Axis);
private:
	class ACharacter* OwnerCharacter;
	class ACPlayer* player;
	bool Used = false;
};
