// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Strategy_Pattern/Operation_Strategy.h"


#include "Patrol_Strategy.generated.h"

/**
 *  ���� ó�� Operation � Enemy �̵� ó���� Patroll
 */
UCLASS()
class PRTFOLIO_12_24_API UPatrol_Strategy : public UObject , public IOperation_Strategy
{	
	GENERATED_BODY()
public:
	virtual FVector Move(FVector Self_Loc) override; // Return Target_Pos
	virtual void Action(class UDataTable* table, class AGameActor* OwnerActor, EMouseBut direction) override;

};
