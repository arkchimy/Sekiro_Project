#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Actors/Builes/CBuild_Book.h"
#include "CAnimInstance_Book.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCAnimInstance_Book : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		EBookCombat combat = EBookCombat::Open;
	ACBuild_Book* Book;
};
