#include "Notifies/Utilities/CSoundAtLoaction.h"
#include "Global.h"

#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

FString UCSoundAtLoaction::GetNotifyName() const
{
	return "CSound_AtLoaction";
}
void UCSoundAtLoaction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!!MeshComp) {}
	else { CLog::Print("MeshComp is Null"); }
	if (!!Sound) {}
	else { CLog::Print("Sound is Null"); }
	if (!!MeshComp->GetOwner()) {}
	else { CLog::Print("MeshComp->GetOwner is Null"); }
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	CheckNull(Sound);
	FVector loc = MeshComp->GetOwner()->GetActorLocation();
	loc.Z += 88;
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound , loc);
	
	CLog::Print(MeshComp->GetOwner()->GetActorLocation());
}