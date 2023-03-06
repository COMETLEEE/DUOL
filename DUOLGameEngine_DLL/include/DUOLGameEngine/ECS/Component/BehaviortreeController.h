/**

	@file      Behaviortree.h
	@brief     비헤이비어 트리의 컨트롤러.
	@details   ~ 초기화, 실행, 정지, 현재 상태 확인 등의 작업을 수행한다.
	@author    SinSeongHyeon
	@date      6.03.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/
#pragma once

#include "DUOLGameEngine/ECS/Component/ComponentBase.h"
#include "DUOLGameEngine/Util/enabled_shared_from_base.h"

namespace BT
{
	class Tree;
}

namespace DUOLGameEngine
{
#pragma once
	class DUOL_GAMEENGINE_API BehaviortreeController : public DUOLGameEngine::ComponentBase
	{
	public:
		BehaviortreeController(DUOLGameEngine::GameObject* owner);

		virtual ~BehaviortreeController() override;

	private:


	};
}
