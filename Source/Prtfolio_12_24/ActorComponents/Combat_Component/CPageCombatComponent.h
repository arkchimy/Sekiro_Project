#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CPageCombatComponent.generated.h"


UENUM(BlueprintType) //			CAnimInstance_Page 외에는 쓰지않아서 독립으로 배치함
enum class EPageCombat : uint8		// Page 오브젝트 
{
	PickUp, S_Back, S_Front, Back, Front, invisible, Max,
	/*
	 PickUp :  Idle
	 S_Back :  뒤를 보이며 Static인 상태
	 S_Front : 앞을 보이며 Static인 상태
	 Back	:	뒤 페이지를 보이는 동작상태
	 Front	:	앞 페이지를 보이는 동작상태
	 invisible	:	안보이는 상태 (다른 페이지를 보고있을때 사용)
	*/
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRTFOLIO_12_24_API UCPageCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPageCombatComponent();

protected:
	virtual void BeginPlay() override;
public:
	FORCEINLINE void Set_Page(class ACPageMaster* InOnwer) { Page = InOnwer; }
	void Set_PickupMode();
	void Set_S_BackMode();
	void Set_S_FrontMode();
	void Set_BackMode();
	void Set_FrontMode();
	void Set_InvisibleMode();

	FORCEINLINE bool Is_PickMode() { return combat == EPageCombat::PickUp; }
	FORCEINLINE bool Is_S_BackMode() { return combat == EPageCombat::S_Back; }
	FORCEINLINE bool Is_S_FrontMode() { return combat == EPageCombat::S_Front; }
	FORCEINLINE bool Is_BackMode() { return combat == EPageCombat::Back; }
	FORCEINLINE bool Is_FrontMode() { return combat == EPageCombat::Front; }
	FORCEINLINE bool Is_InvisibleMode() { return combat == EPageCombat::invisible; }

	FORCEINLINE EPageCombat CurrentMode() { return combat; }
	
private:
	EPageCombat combat = EPageCombat::PickUp;
	class ACPageMaster* Page;
		
};
