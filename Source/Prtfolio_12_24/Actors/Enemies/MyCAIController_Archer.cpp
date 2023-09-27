#include "MyCAIController_Archer.h"
#include "Global.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"


AMyCAIController_Archer::AMyCAIController_Archer()
{
	/*CHelpers::GetAsset(&BT_Asset, "BehaviorTree'/Game/_Character/Enemy/AiController_Type/AiAround/BT_Around.BT_Around'");
	CHelpers::GetAsset(&BB_Asset, "BlackboardData'/Game/_Character/Enemy/AiController_Type/AiAround/BB_Archer.BB_Archer'");
	CLog::Log("Archer_Controller Create");*/
	
}
void AMyCAIController_Archer::BeginPlay()
{
	//Super::BeginPlay();
	//BehaviorTreecmp->StartTree(*BT_Asset);

	//RunBehaviorTree(BT_Asset);
}