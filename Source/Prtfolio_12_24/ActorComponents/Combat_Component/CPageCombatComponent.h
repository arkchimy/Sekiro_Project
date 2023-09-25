#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CPageCombatComponent.generated.h"


UENUM(BlueprintType) //			CAnimInstance_Page �ܿ��� �����ʾƼ� �������� ��ġ��
enum class EPageCombat : uint8		// Page ������Ʈ 
{
	PickUp, S_Back, S_Front, Back, Front, invisible, Max,
	/*
	 PickUp :  Idle
	 S_Back :  �ڸ� ���̸� Static�� ����
	 S_Front : ���� ���̸� Static�� ����
	 Back	:	�� �������� ���̴� ���ۻ���
	 Front	:	�� �������� ���̴� ���ۻ���
	 invisible	:	�Ⱥ��̴� ���� (�ٸ� �������� ���������� ���)
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
