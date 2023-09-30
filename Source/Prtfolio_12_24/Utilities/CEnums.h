#pragma once

#include "CoreMinimal.h"
/*
	#include "Utilities/CEnums.h" 
*/


/*
	===== Monster & Player		=====
*/

UENUM(BlueprintType)
enum class EDirection : uint8		 //	Weapon Type
{
	Front, Back ,Left, Right,Max,
};

UENUM(BlueprintType)
enum class ECombat_Type : uint8		 //	Weapon Type
{
	Unarmed, Fist, SS, Archer, Wizard, Max,
};
UENUM(BlueprintType)
enum class ECombat_Monster : uint8		 //	Weapon Type
{
	Partrol,Melee, Archer, Wizard, Max,
};

UENUM(BlueprintType)
enum class EAttackTag_Type : uint8		 //	Weapon Type
{
	Normal, AirBone , Stun, Counter, Groggy, Max,
};

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle, Action, Hit, Stun, Death, Equip, Interaction, Block, Absolut, Fly, Max,
	/*
		* Idle = 평소의 상태
		* Action = 공격 하고있을 때를 의미
		* Hit = 이미 공격을 맞음( 에어본이 아닌 공격)
		* Stun = 기절 공격을 당한 상태
		* Death = 죽음
		* Equip = 장착 액션 중
		* interaction = 상호작용 오브젝트와 상호작용 중인 상태
		* Block  = 방패를 들고있는 상태 (가드 중)
		* Absolut = 궁극기 사용 (미구현)
		* Fly =  Airbone인 상태
	*/
};

/* 
	===== Player Class	=====
*/
UENUM(BlueprintType)
enum class ECamera_Sight : uint8 // Player 시점 Enum
{
	FPS, TPS,
};

UENUM(BlueprintType)
enum class EMouseBut : uint8 // 마우스 입력
{
	Left, Right,LR,L_LR,R_LR,Shift_Attack,T_Attack,R_Attack,Back_Attack,Front,Back,Max,
};
UENUM(BlueprintType)
enum class EKeybord : uint8 // 키보드 입력
{
	Forward, Back , Left ,Right,Shift,SpaceBar,
};
UENUM(BlueprintType)
enum class EActionBut : uint8 // shift 입력 spacebar 입력
{
	Jump_Key,Shift_F, Shift_B, Shift_L, Shift_R, SpaceBar_F, SpaceBar_B, SpaceBar_L, SpaceBar_R,
};


/*
	===== Monster Class	=====
*/

UENUM(BlueprintType)
enum class ECollisionTags : uint8 //  Monster의 왼손 Attack일떄 왼손 Tag를 가진 Collision만 On/Off하려고 만든 Enum
{
	L_Hand, R_Hand,Shield,Head, Max,
};
UENUM(BlueprintType)
enum class EOperation_Type : uint8
{
	Patrol , Approach , Around, Death,Max
};





class PRTFOLIO_12_24_API CEnum
{

};