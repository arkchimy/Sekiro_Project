#include "Prtfolio_12_24GameModeBase.h"
#include "Global.h"

int APrtfolio_12_24GameModeBase::Page_Cnt = 0;

APrtfolio_12_24GameModeBase::APrtfolio_12_24GameModeBase()
{
	Page_Cnt = 0;
	ConstructorHelpers::FClassFinder<APawn> pawn(L"Blueprint'/Game/_Character/_Player/BP_CPlayer.BP_CPlayer_C'");
	if (pawn.Succeeded())
		DefaultPawnClass = pawn.Class;

	CHelpers::GetClass<APawn>(&DefaultPawnClass, L"Blueprint'/Game/_Character/_Player/BP_CPlayer.BP_CPlayer_C'");
	
}