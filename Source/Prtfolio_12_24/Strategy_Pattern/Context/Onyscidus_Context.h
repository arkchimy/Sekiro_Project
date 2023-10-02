// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Strategy_Pattern/Operation_Context.h"
#include "Onyscidus_Context.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UOnyscidus_Context : public UOperation_Context
{
	GENERATED_BODY()
public:
	UOnyscidus_Context();
	virtual void Action(FVector Target_Location, class AGameActor* Target = nullptr) override;
};
