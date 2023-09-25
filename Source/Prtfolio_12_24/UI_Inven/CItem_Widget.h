// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"
#include "DataTable/FSlot_Info.h"
#include "CItem_Widget.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCItem_Widget : public UUserWidget
{
	GENERATED_BODY()
public:
	//Init
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UCUserWidget_Inventory* Inventory;
public:
	//Edit
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		class UTexture2D* Img;
public:
	void Set_Info(FSlot_Info input);
	FSlot_Info Get_Info();
	void Insert_Slot(FVector2D pos,class UCUserWidget_Slot* slot);
	
public:
	// Implement
	UFUNCTION(BlueprintImplementableEvent)
		void View_Item(bool inval);
	UFUNCTION(BlueprintImplementableEvent)
		void Img_Hitable(bool inval);
public:
	// BluePrint Callable
	UFUNCTION(BlueprintCallable)
		void Double_ClickEvent();
	UFUNCTION(BlueprintCallable)
		void All_Slot_Hit(bool val);
	UFUNCTION(BlueprintCallable)
		void Throw_on_thefloor();
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		FSlot_Info info;
	UPROPERTY(BlueprintReadOnly)
		class UCUserWidget_Slot* Current_Slot;
	
};
