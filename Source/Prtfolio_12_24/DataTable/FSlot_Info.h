#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "CCombatData.h"
#include "FSlot_Info.generated.h"




UENUM()
enum class ESlotType : uint8
{
	Head,	// Head Armor
	Sheath_Weapon,	// Ex ) Sword , Katana
	Upper_Weapon,	// Bow , GreatSword
	Body,		// Armor
	QuickSlot,	// Portion Item 
	Etc1_1,
	Etc1_2,
	Etc2_1,
	Etc2_2,
	Max,
};

USTRUCT(BlueprintType)
struct FDLinklist_Slot
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UCUserWidget_Slot* Left_Slot;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCUserWidget_Slot* Right_Slot;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCUserWidget_Slot* Up_Slot;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCUserWidget_Slot* Down_Slot;
};

USTRUCT(BlueprintType)
struct FSlot_Info : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		int id = 0;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UTexture2D* Img;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FName Name;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		ESlotType SlotType = ESlotType::Max;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		UCCombatData* Attackment_class;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USoundCue* DragDetect_Sound;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USoundCue* Drop_Sound;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class USoundCue* Use_Sound;
	
	// 가격
	// Sound[3]   -  Pick - put  - Equip Sound

	// 공격력
	
};
