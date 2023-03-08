#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_MakeNoise.h"

namespace DUOLGameEngine
{
	BT::NodeStatus DUOLGameEngine::Action_MakeNoise::tick()
	{
		// 사운드 재생 함수 넣을 것.
		// 노이즈는 다른 사운드로 변경할 경우가 없을 듯 하니 포트는 불필요하다.
		return BT::NodeStatus::SUCCESS;
	}
}
