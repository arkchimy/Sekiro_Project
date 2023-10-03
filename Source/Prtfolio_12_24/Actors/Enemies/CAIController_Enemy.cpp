#include "CAIController_Enemy.h"
#include "Global.h"

#include "Actors/GameActor.h"
#include "Actors/CEnemy.h"
#include "Actors/CPlayer.h"


#include "ActorComponents/State_Component/CStateComponent.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"


#include "Strategy_Pattern/Context/Onyscidus_Context.h" // Onyscidus_Context
#include "Strategy_Pattern/Operation_Context.h"	//Master_Context



ACAIController_Enemy::ACAIController_Enemy()
{
	
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "PerCeption");

	SetPerception();
}


ACAIController_Enemy::~ACAIController_Enemy()
{
	CheckNull(oper);
	UPatrol_Strategy* temp = Cast<UPatrol_Strategy>(oper);
	temp->ConditionalBeginDestroy();
	oper = nullptr;
}

//InitializeParentChain
void ACAIController_Enemy::BeginPlay()
{
	Super::BeginPlay();
	State = CHelpers::GetComponent<UCStateComponent>(GetPawn());


	FVector target_location;

	AGameActor* GameActor = Cast<AGameActor>(GetPawn());
	if(!!GameActor)
		TeamID = GameActor->GetGenericTeamId();
	if(!!Context_Class)
		Context = NewObject<UOperation_Context>(this, Context_Class);
	if (!!Context)
	{
		Context->Init_Context(GameActor);
		oper = NewObject<UPatrol_Strategy>(Context,FName("Context_"), RF_MarkAsRootSet);
		Context->Set_Operation(oper);

		target_location = Context->Move(GetPawn()->GetActorLocation());
	}
}


void ACAIController_Enemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}


void ACAIController_Enemy::SetPerception()
{
	CHelpers::CreatePerception<UAISenseConfig_Sight>(this, &SightConfig, "Sight Config");

	SetPerceptionComponent(*Perception);

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController_Enemy::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void ACAIController_Enemy::Random_Select_Operation()
{
	//int ran = rand() % int32(EOperation_Type::Max);
	
	int ran = rand() % 3;
	switch(ran)
	{
	case 0:
		Operation_Type = EOperation_Type::Approach;
		break;
	case 1:
		Operation_Type = EOperation_Type::Around;
		break;

	default:
		Operation_Type = EOperation_Type::Around;
	}
}
void ACAIController_Enemy::Set_FocusActor(AActor* Attacker)
{
	//SetFocus(Attacker, EAIFocusPriority::Move);
	//ClearFocus(EAIFocusPriority::Default);
	ACEnemy* enemy = Cast<ACEnemy>(GetPawn());
	if (!!enemy)
		enemy->KnockBack_AddOffset(Attacker);
	
}

bool ACAIController_Enemy::interp_Focus(FVector targetlotation, float deltatime)
{
	FRotator current = GetPawn()->GetActorRotation();
	FRotator goal = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), targetlotation);
	FRotator rot = UKismetMathLibrary::RInterpTo(current, goal, deltatime, 0.01f);
	
	GetPawn()->SetActorRotation(FRotator(current.Pitch,rot.Yaw,rot.Roll));
	SetControlRotation(rot);

	if (abs(current.Yaw - rot.Yaw) <= 0.1f)
	{
		return true;
	}
	return false;
}

void ACAIController_Enemy::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed()) 
	{
		Blackboard->SetValueAsObject("TargetActor", Actor);
			
		UKismetSystemLibrary::K2_ClearTimerHandle(this, target_handler);

		CheckFalse(Operation_Type == EOperation_Type::Patrol); // 패트롤중이라면 실행

		UObject* BB_Target = Blackboard->GetValueAsObject("TargetActor");
		AGameActor* target = Cast<AGameActor>(BB_Target);
		if (!!target) // Target이 Null이 아니라면
		{	// Approach 적용 시점
			Operation_Type = EOperation_Type::Approach;
			ACEnemy* enemy = Cast<ACEnemy>(GetPawn());
			if (!!enemy)
			{	// LookAt Target Notify 할떄 필요함
				enemy->Set_TargetActor(target);
			}
		
		}
	}
	else
		target_handler = UKismetSystemLibrary::K2_SetTimer(this, "Update_Target", 5.0f,false);
}

ETeamAttitude::Type ACAIController_Enemy::GetTeamAttitudeTowards(const AActor & Other) const
{
	const APawn* pawn = Cast<APawn>(&Other);
	const ACEnemy* enemy = Cast<ACEnemy>(pawn);
	const ACPlayer* player = Cast<ACPlayer>(pawn);

	if(!!enemy)
	{
		class IGenericTeamAgentInterface* Bot_Tid;
		Bot_Tid = Cast<IGenericTeamAgentInterface>(enemy->GetController());
		ACAIController_Enemy* bot_id = Cast<ACAIController_Enemy>(enemy->GetController());
		if(!!bot_id)
		{
			
			if (TeamID == bot_id->GetGenericTeamId())
				return ETeamAttitude::Friendly;
			else
				return ETeamAttitude::Hostile;
		}

		if (!!Bot_Tid)
		{
			if (TeamID == Bot_Tid->GetGenericTeamId())
				return ETeamAttitude::Friendly;
			else
				return ETeamAttitude::Hostile;
		}

	}
	else if(!!player)
	{
		//Aicontoroll 가 없
		auto player_Tid = Cast<IGenericTeamAgentInterface>(player);
		if (!!player_Tid)
		{
			if (TeamID != player_Tid->GetGenericTeamId())
				return ETeamAttitude::Hostile;
		}
	}
	return ETeamAttitude::Neutral;
}

void ACAIController_Enemy::BlackBoard_Update()
{

}
void ACAIController_Enemy::MoveUpdate(bool inval)
{
	//BlackBoradCmp->SetValueAsBool("bCanMove", inval);
}
void ACAIController_Enemy::Damaged_Target(AActor* Attacker)
{
	//BlackBoradCmp->SetValueAsObject("TargetActor", Attacker);
	Operation_Type = EOperation_Type::Approach;


	CheckFalse(Operation_Type == EOperation_Type::Patrol); // 패트롤중이라면 실행

	//AGameActor* target = Cast<AGameActor>(Attacker);
	//if (!!target) // Target이 Null이 아니라면
	//	Operation_Type = EOperation_Type::Approach;
	
}
void ACAIController_Enemy::Update_Target()
{
	
	UObject* BB_Target = Blackboard->GetValueAsObject("TargetActor");
	AActor* BB_Player = Cast<AActor>(BB_Target);
	
	if (!!BB_Player) 
	{
		FVector location = BB_Player->GetActorLocation();
		Blackboard->SetValueAsVector("MoveTo_Location", location);
		//Blackboard->SetValueAsObject("TargetActor", NULL);

		
	}

}
void ACAIController_Enemy::Monster_Dead()
{
	/*CheckTrue(Operation_Type == EOperation_Type::Death);
	Operation_Type = EOperation_Type::Death;
	BlackBoradCmp->SetValueAsEnum(FName("Operation"), int8(Operation_Type));
	BehaviorTreecmp->StopTree();
	ClearFocus(EAIFocusPriority::Default);*/
}

void ACAIController_Enemy::Monster_Action(FVector Loc)
{
	//CheckNull(Context);
	if (!!Context)
	{
		Context->Action(Loc);
		return;
	}
	CLog::Print("Context is null");
}
