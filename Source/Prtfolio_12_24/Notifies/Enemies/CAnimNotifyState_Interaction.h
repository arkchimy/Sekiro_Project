#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_Interaction.generated.h"

/**
 * interaction need Value On/Off
 */
UCLASS()
class PRTFOLIO_12_24_API UCAnimNotifyState_Interaction : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName() const;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration);
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	class ACEnemy* enemy;
	
};
