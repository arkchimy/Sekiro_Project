#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Actor.h"
#include "Actors/Builes/CBuild.h"

#include "CPageMaster.generated.h"

UCLASS()
class PRTFOLIO_12_24_API ACPageMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPageMaster();
	virtual ~ACPageMaster();
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		FORCEINLINE int ID() { return id; }
	FORCEINLINE int Order() { return order; }
	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE class UMaterialInstanceDynamic* Get_DynamicMat() { return dynamic_mat; }
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UMaterialInstanceDynamic* dynamic_mat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UCPageCombatComponent* CombatComp;

	UFUNCTION(BlueprintCallable)
		void Page_In_Book(class ACBuild_Book* book);
	UFUNCTION(BlueprintCallable)
		void Page_Update(bool NextPage);
	UFUNCTION(BlueprintCallable)
		void Set_Build(class ACBuild* build);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ID_init(int val);
public:
	FORCEINLINE USkeletalMeshComponent* Get_Mesh() { return mesh; }
	FORCEINLINE ACBuild* Get_Build() { return Obj_class; }
	
	void Init_MeshLocation(class ACBuild_Book* book);

	bool Is_PickMode();
	bool Is_S_BackMode();
	bool Is_S_FrontMode();
	bool Is_BackMode();
	bool Is_FrontMode();
	bool Is_InvisibleMode();
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		USkeletalMeshComponent* mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UBoxComponent* collision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> Front_widgetClass; // Widget c로 만들어서 TSub편하게
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> Back_widgetClass;
	
private:
	int id; // 어떤 Book 인지 판별
	int order;  // 책의 Order 연결리스트의 우선순위
	class UMaterialInstanceConstant* mesh_mat;
	class ACBuild_Book* OwnerBook;

	class ACBuild* Obj_class;

	//bool bLookatPage = false;
	//bool bfront = true;
	//bool bpickstate = false;


};
