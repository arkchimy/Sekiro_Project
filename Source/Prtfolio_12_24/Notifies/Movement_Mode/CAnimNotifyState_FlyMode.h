#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_FlyMode.generated.h"

/**
 *
 */
UCLASS()
class PRTFOLIO_12_24_API UCAnimNotifyState_FlyMode : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName() const;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	class ACharacter* OwnerCharacter;
};
