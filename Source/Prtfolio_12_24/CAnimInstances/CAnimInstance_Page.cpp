#include "CAnimInstance_Page.h"
#include "Global.h"
#include "Actors/Builes/CPageMaster.h"


void UCAnimInstance_Page::NativeBeginPlay()
{

	Page = Cast<ACPageMaster>(GetOwningActor());
	Super::NativeBeginPlay();
}

void UCAnimInstance_Page::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//CheckNull(Owner_Book);

	if (!!Page) 
	{
		combat = CombatComp->CurrentMode();
		
	}
	else
	{
		Page = Cast<ACPageMaster>(GetOwningActor());
		if(!!Page)
			CombatComp = CHelpers::GetComponent<UCPageCombatComponent>(Page);
	}

}

