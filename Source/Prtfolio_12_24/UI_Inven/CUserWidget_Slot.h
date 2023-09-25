#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"

#include "DataTable/FSlot_Info.h"


#include "CUserWidget_Slot.generated.h"




UCLASS()
class PRTFOLIO_12_24_API UCUserWidget_Slot : public UUserWidget
{
	GENERATED_BODY()
public:
	//init
	void Set_OwnerInventory(class UCUserWidget_Inventory* Owner_Inven);
public:
	// BluePrint Interaction
	UFUNCTION(BlueprintCallable)
		virtual void UnEquip() {};
	UFUNCTION(BlueprintCallable)
		virtual void Equip(class UCItem_Widget* item) {};
	UFUNCTION(BlueprintCallable)
		virtual void Slot_Swap(class UCUserWidget_Slot* swap_slot);
	UFUNCTION(BlueprintCallable)
		virtual void Slot_Use();	// Slot Use

	UFUNCTION(BlueprintCallable)
		void Insert_Item(class UCItem_Widget* input_item);// Item_Widget => Slot Insert

	UFUNCTION(BlueprintImplementableEvent)
		void Slot_Update(class UTexture2D* slot_img); // Img Update
	UFUNCTION(BlueprintCallable)
		void DragOver_Slot(class UCItem_Widget* Insert_Item);
	UFUNCTION(BlueprintCallable)
		void DragLeave_Slot(class UCItem_Widget* Insert_Item);


	UFUNCTION(BlueprintImplementableEvent)
		void Possible_Tint();
	UFUNCTION(BlueprintImplementableEvent)
		void Impossible_Tint();
	UFUNCTION(BlueprintImplementableEvent)
		void Reset_Tint();

	void SlotHasItem_Visible(bool val);
public:
	//EquipSlot
	virtual void Equip_Item(class UCItem_Widget* item) {};
public:	
	bool Possible_Check(class UCItem_Widget* Insert_Item);// 위 결과에 따라 드래그 이동이 안됌
	bool IsValid_Slot(class UCUserWidget_Slot* isvalid, UCItem_Widget* Same_Widget);

	void Set_info(FSlot_Info* data);
	void Slot_Position(FVector2D pos);
	FVector2D Slot_Position() { return Slot_pos; }
	
	void Set_LinkItemWidget(class UCItem_Widget* input_Item);
	void Reset_LinkItemWidget(ESlotType type);

	FORCEINLINE void Set_ItemWidget(class UCItem_Widget* input_Item) { Item_Widget = input_Item; }
	FORCEINLINE class UCItem_Widget* Get_ItemWidget() { return Item_Widget; }

	bool IsUsed_Slot();
	void Setting_EquipSlot(ESlotType input); //  EquipSlot Slot Number
	
public:
	// FORCEINLINE 
	FORCEINLINE FSlot_Info* Get_info() { return &info; }
	 
	/*FORCEINLINE UTexture2D* Img() { return img; }
	FORCEINLINE void Img(UTexture2D* input) { img = input; }*/
protected:
	
	class UCUserWidget_Inventory* Inventory;
	class UCItem_Widget* Item_Widget = nullptr; // 아이템 이미지를 가지고있음.
	//class UTexture2D* img;

	bool Used_Slot = false;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FSlot_Info info;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Linklist")
		FVector2D Slot_pos;
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Linklist")
		FDLinklist_Slot Dlink_Slot;
	bool bTreasureSlot = true;
private:
	
};

