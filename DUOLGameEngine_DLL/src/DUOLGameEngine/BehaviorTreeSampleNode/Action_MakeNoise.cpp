#include "DUOLGameEngine/BehaviorTreeSampleNode/Action_MakeNoise.h"

namespace DUOLGameEngine
{
	BT::NodeStatus DUOLGameEngine::Action_MakeNoise::tick()
	{
		// ���� ��� �Լ� ���� ��.
		// ������� �ٸ� ����� ������ ��찡 ���� �� �ϴ� ��Ʈ�� ���ʿ��ϴ�.
		return BT::NodeStatus::SUCCESS;
	}
}
