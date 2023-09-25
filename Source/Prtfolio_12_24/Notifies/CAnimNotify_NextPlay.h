
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_NextPlay.generated.h"

/**
 * 
 */
UCLASS()
class PRTFOLIO_12_24_API UCAnimNotify_NextPlay : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName() const;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

	class ACharacter* OwnerCharacter;
	class ACPlayer* Player;
	class UCombatComponent* Combatcomp;

	
};
