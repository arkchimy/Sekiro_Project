#include "CAnimInstance_Book.h"
#include "Global.h"

void UCAnimInstance_Book::NativeBeginPlay()
{

	Book = Cast<ACBuild_Book>(GetOwningActor());
	Super::NativeBeginPlay();
}

void UCAnimInstance_Book::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//CheckNull(Owner_Book);

	if (!!Book)
	{
		combat = Book->CurrentCombat();

	}
	else
	{
		Book = Cast<ACBuild_Book>(GetOwningActor());
	
	}

}

