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
		* Idle = ����� ����
		* Action = ���� �ϰ����� ���� �ǹ�
		* Hit = �̹� ������ ����( ����� �ƴ� ����)
		* Stun = ���� ������ ���� ����
		* Death = ����
		* Equip = ���� �׼� ��
		* interaction = ��ȣ�ۿ� ������Ʈ�� ��ȣ�ۿ� ���� ����
		* Block  = ���и� ����ִ� ���� (���� ��)
		* Absolut = �ñر� ��� (�̱���)
		* Fly =  Airbone�� ����
	*/
};

/* 
	===== Player Class	=====
*/
UENUM(BlueprintType)
enum class ECamera_Sight : uint8 // Player ���� Enum
{
	FPS, TPS,
};

UENUM(BlueprintType)
enum class EMouseBut : uint8 // ���콺 �Է�
{
	Left, Right,LR,L_LR,R_LR,Shift_Attack,T_Attack,R_Attack,Back_Attack,Front,Back,Max,
};
UENUM(BlueprintType)
enum class EKeybord : uint8 // Ű���� �Է�
{
	Forward, Back , Left ,Right,Shift,SpaceBar,
};
UENUM(BlueprintType)
enum class EActionBut : uint8 // shift �Է� spacebar �Է�
{
	Jump_Key,Shift_F, Shift_B, Shift_L, Shift_R, SpaceBar_F, SpaceBar_B, SpaceBar_L, SpaceBar_R,
};


/*
	===== Monster Class	=====
*/

UENUM(BlueprintType)
enum class ECollisionTags : uint8 //  Monster�� �޼� Attack�ϋ� �޼� Tag�� ���� Collision�� On/Off�Ϸ��� ���� Enum
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