#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Actors/GameActor.h"


#include "CPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCameraMove1, float, Axis);



UCLASS()
class PRTFOLIO_12_24_API ACPlayer : public AGameActor
{
	GENERATED_BODY()
public: // Delegate 
	FCameraMove1 CameraDelegate_H; // 3인칭 카메라와 1인칭 카메라 무빙
	FCameraMove1 CameraDelegate_V;
public:
	ACPlayer();
protected:
	virtual void BeginPlay() override;
	void Init_Player();
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void View_EnemyUI(class AGameActor* target) override;
protected:
	/*
	* ==================  Edit
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> HUD_class; // HUD BluePrint

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		ECamera_Sight sight = ECamera_Sight::TPS;
	/*
	* ==================  Component
	*/
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USkeletalMeshComponent* FP_Mesh;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USceneComponent* Book_SceneComponent;
	/*
	* ==================  ActorComponent
	*/
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActorComponent_Target* Target_Comp;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TSubclassOf<class ACCamera_Manager> CameraManager_Class;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCameraShakeBase> WalkShake;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<UAnimInstance> FP_animInstance;

public:
	
	

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Build_Book")
		TSubclassOf<class ACBuild_Book> BP_buildBook_Class;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float TurnSpeed = 3.f;
	
public:
	FORCEINLINE class USkeletalMeshComponent* Get_FP_Mesh() { return FP_Mesh; }
	FORCEINLINE class UCameraComponent* GetCamera() { return Camera; } // 화살 줌 떙길떄 씀
	
	FORCEINLINE int Get_WheelCnt() { return wheel_Cnt; }
	FORCEINLINE void WheelCnt(int val) { wheel_Cnt = val; }
	
	FORCEINLINE ECamera_Sight Get_Sight() { return sight; }


	//상태이상
	void Stren_Damaged(ACharacter* Attacker, FString CC_Option);
	void Add_ShowActor(AActor* actor);
	void Remove_ShowActor(AActor* actor);


	UFUNCTION(BlueprintCallable)
		FORCEINLINE class ACBuild_Book* Get_BuildBook() { return build_book; }
	UFUNCTION(BlueprintImplementableEvent)
		void DeathMode_Cine_Play();

	//SS_ 돌진하는데 사용된 함수들
	void Forward_Rush(float Axis);
	void Set_Max_Speed(float inval);

private:
	void Move_Walk();
	void Move_Run();
	void Move_Forward(float Axis);
	void Move_Right(float Axis);

	void Look_Horizontal(float Axis);
	void Look_Vertical(float Axis);

	void Character_ForwardRotation(const FVector Cross, float Axis); // TPS 전,후진할때 회전 
	void Character_Rotation(const float dot ,  const FVector Cross , float Axis);
	
	void Find_Target();
	void Input_SpaceBar();

	bool bFight_Mode = false; // 무기 꺼냈는지 체크  임시 

	virtual void OnFist() override;
	virtual void OnSS() override;
	virtual void OnArcher() override;
	virtual void OnWizard() override;

	void Weapon_Change(ECombat_Type type);

	void L_Attack();
	void L_AttackRelease();
	void R_Attack();
	void R_AttackRelease();
	void Do_Parrying();

	void Absolut_Attack();

	/*
	* Interaction Action
	*/
	void Open_Inventory();
	virtual void Open_TreasureBox(TArray<FInven_Item> Treasureitems, class ACTreasureBox_Master* box) override;
	virtual void Close_TreasureBox() override;
	void Open_BuildBook();
	void Select_Build();

	void Build_Rotation(float Axis);
	void PickUp();
	void InteractionBuild_Update();
private:
	bool L_But = false;
	bool R_But = false;

public:
	FORCEINLINE void Block_Success() { R_Attack(); } // Block NextMontage Play

	UFUNCTION(BlueprintCallable)
		void Set_View_Fallow_Camera();
	void Set_Fallow_Camera_Rotation(FRotator rot);

	UFUNCTION(BlueprintCallable)
		void Set_View_FPS_Camera();
	UFUNCTION(BlueprintCallable)
		void Set_View_Owner_Camera();
	UFUNCTION(BlueprintCallable)
		void Set_View_Dead_Camera();
	UFUNCTION(BlueprintCallable)
		void Set_View_Skill_Camera();
	UFUNCTION(BlueprintCallable)
		void Slot_Swap(class ACWeaponment* Weaponment);
	UFUNCTION(BlueprintCallable)
		void Player_Move(bool val);
	


	void Set_Interaction_Build(class ACBuild* build);
	class ACBuild* Get_Interaction_Build() { return interact_Build; }
private:
	int wheel_Cnt = 1;

private:
	UPROPERTY(EditAnywhere)
		class ACCamera_Manager* Camera_Manager;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class USceneCaptureComponent* MeshWidget;
public:
	
	void SetCrossHair(FCrossHairSet crosshair);

	
public:

	UFUNCTION(BlueprintImplementableEvent)
		void HUDCrossHair(FCrossHairSet data);
	UFUNCTION(BlueprintImplementableEvent)
		void Archer_CrossHair(bool val);//1 = 쪼이기 0 = 원래대로
	UFUNCTION(BlueprintImplementableEvent)
		void Hit_CrossHair();
	UFUNCTION(BlueprintImplementableEvent)
		void Kill_CrossHair();
	UFUNCTION(BlueprintImplementableEvent)
		void Attached_CrossHair();
	UFUNCTION(BlueprintImplementableEvent)
		void Damaged_Event();
	UFUNCTION(BlueprintCallable)
		void T_AttackRelease();

	virtual class AGameActor* Get_TargetActor() override;
	virtual bool Target_Dot() override;
	void Camera_Setting(ECamera_Sight str);  // "Fps Tps

private:

	FString CC_State;
	class ACharacter* Hold_Attacker;

	int resiste_Cnt = 0;

	class ACBuild_Book* build_book;
	class ACBuild* interact_Build;
	
	//TPS 카메라 Rotation 
	void Pressed_Alt();
	void Released_Alt();

	
	void Play_Action(EKeybord lastkey, EKeybord EventKey);
	UFUNCTION()
		void Jump_End();
protected:
	UPROPERTY(BlueprintReadOnly)
		class UCGame_HUD* HUD;
private:
	

	APlayerController* Playercontroll;
};