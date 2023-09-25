#pragma once
#include "CoreMinimal.h"
#include "Actors/CWeaponment.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "CUserWidget_Slot.h"
#include "Engine/DataTable.h"

#include "DataTable/FSlot_Info.h"
#include "UI_Inven/CItem_Widget.h"

#include "CUserWidget_Inventory.generated.h"

UENUM(BlueprintType)
enum class EPanelTarget : uint8
{
	Inven,
	TreasureBox,
};

UCLASS()
class PRTFOLIO_12_24_API UCUserWidget_Inventory : public UUserWidget
{
	GENERATED_BODY()
public:
	/*
	* Edit
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "DataTable")
		UDataTable* DT_Item;
	/* Inventory
	{
	{0,0,0,1,1,0,0,0}, // 빈공간이면 0
	{0,0,0,1,1,0,0,0}, // 사용중이면 1로 표시할 예정
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	}
	 */
public:
	// Construct Event Init
	void Init_MouseControl(class APlayerController* Controller);
	UFUNCTION(BlueprintCallable)
		void Init_InvenSlot();
	UFUNCTION(BlueprintCallable)
		void Init_TreasureSlot();
	UFUNCTION(BlueprintCallable)
		void Init_EquipSlot(TArray<UCUserWidget_Slot*> Head, TArray<UCUserWidget_Slot*> Sheath, TArray<UCUserWidget_Slot*> Upper, TArray<UCUserWidget_Slot*> body, TArray<UCUserWidget_Slot*> quick);
	

	void Slot_xLink(TArray<UCUserWidget_Slot*> slot_array ,int Width , int pos_Index);
	void Slot_yLink(TArray<UCUserWidget_Slot*> slot_array, int Width);

	void Slot_xLink(); // inven 전용
	void Slot_yLink();	// inven 전용

	void Slot_xLink(int Slots_Row, int Slots_Column); // Treasureinven 전용
	void Slot_yLink(int Slots_Row, int Slots_Column);	// Treasureinven 전용
	
public:
	//Begin Event
	
	UFUNCTION(BlueprintCallable)
		void Slot_Swap(UCUserWidget_Slot* slot1, UCUserWidget_Slot* slot2);
	UFUNCTION(BlueprintCallable)
		void EquipSlot_Swap(UCUserWidget_Slot* slot1, UCUserWidget_Slot* slot2);

	// Call BulePrint
	UFUNCTION(BlueprintImplementableEvent)
		void Create_ItemWidget(FSlot_Info data);
	UFUNCTION(BlueprintImplementableEvent)
		void Create_OtherWidget(FSlot_Info data);
	UFUNCTION(BlueprintImplementableEvent)
		void Create_TempWidget(FSlot_Info data); // Pick Before
	UFUNCTION(BlueprintImplementableEvent)
		UDataTable* Get_ItemDB();

	UFUNCTION(BlueprintImplementableEvent)
		void TreasurePanel_Visible(bool val);
	UFUNCTION(BlueprintImplementableEvent)
		void InvenPanel_Visible(bool val);

public:
	UFUNCTION(BlueprintCallable)
		void All_Slot_Hit(bool val);


public:
	//FORCEINLINE
	FORCEINLINE class AGameActor* GetOwnerActor() { return OwnerActor; }
	FORCEINLINE void Set_TreasureBox(class ACTreasureBox_Master* box) {TouchTreasure = box; }
public:
	AActor* Get_Owner();
	void Inventory_Update(TArray<FInven_Item> inven_item);

	void TreasureBox_Update(TArray<FInven_Item> Treasures);
	void Treasure_Add(int id);
	void Treasure_Remove(int id);		
	void Throw_InvenItem(int id);	// Inven Slot=> Floor
	void Throw_TreasureItem(int id);	// Treasure Box => Floor
	bool Item_PickUp(int id); //  item Pick 
	

	void Reset_ItemWidget(class UCItem_Widget* widget);
	void Reset_AllSlotTint();

	void Open_Inventory();
	void Open_TreasureBox();
	void Close_TreasureBox();
	UCUserWidget_Slot* Find_Slot(TArray<UCUserWidget_Slot*> slots , class UCItem_Widget* Item_info);
	
	void Slot_Use(class UCItem_Widget* UsedSlot);
	void EquipSlot_Use(UCUserWidget_Slot* Useslot);
	TPair<int, int> Slot_Size(const FSlot_Info info);	// Slot Size Return

	void SlotHasItem_Visible(bool val, EPanelTarget target);
//public:
//	void Inventory_Update()
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<UCUserWidget_Slot*> Inven_Slots; // item object
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<UCUserWidget_Slot*> Treasure_Slots;
	UPROPERTY(BlueprintReadOnly)
		class APlayerController* Mouse;
private:

	
	TArray<UCUserWidget_Slot*> Equip_Slots[5];
	
	
	TArray<FVector2D> EquipSlot_Pos =
	{
	FVector2D(120,-130),
	FVector2D(10, -20), //Sheath
	FVector2D(370, -30),//Upper
	FVector2D(150, 0),//Body
	FVector2D(0, 0),
	};

	

	class AGameActor* OwnerActor;
	class ACTreasureBox_Master* TouchTreasure;

	FSlot_Info item;
	int current_panel = 0;

	int Row = 5;
	int Column = 7;
	int Treasure_Row = 4;
	int Treasure_Column = 5;

	class UCUserWidget_Slot* inventory[5][7];
	class UCUserWidget_Slot* Treasure_inven[4][5];
protected:
	//Monitor
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		class UCItem_Widget* Temp_Widget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<class UCItem_Widget*> Item_widgets;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<class UCItem_Widget*> Treasure_Items;


protected:
	// Pannel Position Init
	FVector2D Equip_PanelPos = FVector2D(350, -350);

	float TreasureSlot_X = -705.0;	// TreasureInven FirstPos.X
	
	float TreasureSlot_Y = -425.f;	// TreasureInven FirstPos.Y


	float Slot_X = 312.5f;	//Inven FirstPos.X
	
	float Slot_Y = 45.0f;	//Inven FirstPos.Y
	
	float Slot_RangeX = 135.f * 0.6f;  // 81
	
	float Slot_RangeY = 135.f * 0.6f;

	bool Do_once = true;
	bool bInven_Panel = false;
};
