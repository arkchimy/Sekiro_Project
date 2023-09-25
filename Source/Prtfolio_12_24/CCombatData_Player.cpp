#include "CCombatData_Player.h"
#include "Global.h"
#include "Actors/CPlayer.h"
#include "ActorComponents/State_Component/CStateComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"


void UCCombatData_Player::BeginPlay(class ACharacter* InOwnerCharacter)
{
	Super::BeginPlay(InOwnerCharacter);
	player = Cast<ACPlayer>(OwnerCharacter);

	CheckNull(Action_Table);
	TArray<FName> Row_Name = Action_Table->GetRowNames();
	Action = *(Action_Table->FindRow<FPlayer_Action>(Row_Name[0], "OneHand_Action"));

}

void UCCombatData_Player::Play_Action(const EActionBut but)
{
	FAttack_Combo data;
	switch(but)
	{
		case EActionBut::Jump_Key:
			data = Action.Jump_Key;
			break;

		case EActionBut::Shift_F :
			data = Action.Shift_F;
			break;
		case EActionBut::Shift_B:
			data = Action.Shift_B;
			break;
		case EActionBut::Shift_L:
			data = Action.Shift_L;
			break;
		case EActionBut::Shift_R:
			data = Action.Shift_R;
			break;

		case EActionBut::SpaceBar_F:
			data = Action.SpaceBar_F;
			break;
		case EActionBut::SpaceBar_B:
			data = Action.SpaceBar_B;
			break;
		case EActionBut::SpaceBar_R:
			data = Action.SpaceBar_R;
			break;
		case EActionBut::SpaceBar_L:
			data = Action.SpaceBar_L;
			break;
		default :
			data.Montage = nullptr;
	}
	CheckNull(data.Montage); // case 에 없었다면 
	CheckNull(player);
	if (but == EActionBut::Shift_F) 
	{
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(player);
		if(!!state)
			state->Set_Rush(true);
	}
	player->PlayAnimMontage(data.Montage, data.PlayRatio, data.StartSection);
	player->Player_Move(data.bCanMove);

	CheckFalse(but == EActionBut::Jump_Key); // 점프키 였을 경우.
	
	
}

void UCCombatData_Player::Jump_End()
{

	CheckNull(player);
	CheckTrue(player->GetCharacterMovement()->IsFlying()); // 날고있으면 return

	
	player->PlayAnimMontage(Action.Jump_Key.Montage, Action.Jump_Key.PlayRatio * 3, FName("End"));
	UKismetSystemLibrary::K2_ClearTimer(this, "Jump_End");
}
